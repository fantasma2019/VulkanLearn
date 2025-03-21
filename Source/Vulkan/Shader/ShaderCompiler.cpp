#include "ShaderCompiler.h"

#include <fstream>

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Core/Log/Log.h"
#include "Core/Container/String.h"

namespace Pulse
{
    CacheCompileResult ShaderCompiler::CacheCompile(const File::Path& filePath, u8 flags)
    {
        // 预处理
        CacheCompileResult result;
        auto processResult = PreprocessShaderFile(filePath);

        if (!Utils::Shader::CheckShaderPreprocessResult(processResult))
        {
            PL_ASSERT(false, "Shader File Preprocess ERROR: " + processResult.ErrorMessage);
            return {};
        }
        result.ShaderName = processResult.Name;
        // 编译
        if (flags & ShaderAPIFlags::DirectX && flags & ShaderAPIFlags::None)
        {
            PL_ASSERT(false, "UnSupport ShaderAPI Type!");
            return {};
        }
        if (flags & ShaderAPIFlags::Vulkan)
        {
            result.Sources = CompileVulkanBinaries(processResult);
            result.Success = true;
            return result;
        }
        if (flags & ShaderAPIFlags::OpenGL)
        {
            result.Sources = CompileOpenGLBinaries(processResult);
            result.Success = true;
            return result;
        }
        return {};
    }

    SPIRVBinaryDatas ShaderCompiler::CompileVulkanBinaries(const ShaderPreprocessResult& processResult)
    {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
        constexpr bool optimize = true;
        if (optimize)
            { options.SetOptimizationLevel(shaderc_optimization_level_performance);}
        File::Path cacheDir = Utils::Shader::GetShaderCacheDir();

        SPIRVBinaryDatas outBinaries;
        for (auto&& [stage, source] : processResult.Sources)
        {
            File::Path cachedPath = cacheDir / (processResult.Name + Utils::Shader::ShaderStageCachedVulkanFileExtension(stage));
            std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0,std::ios::end);
                const size_t size = in.tellg();
                in.seekg(0,std::ios::beg);

                auto& data = outBinaries[stage];
                data.resize(size / sizeof(u32));
                in.read(reinterpret_cast<char*>(data.data()), size);
            }
            else
            {
                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::Shader::GetShaderKind(stage), processResult.Name.c_str(), options);
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    PL_ASSERT(false, "Shader Compilation Error: " + module.GetErrorMessage());
                    return {};
                }

                outBinaries[stage] = {module.cbegin(), module.cend()};

                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& binaries = outBinaries[stage];
                    out.write(reinterpret_cast<char*>(binaries.data()), binaries.size() * sizeof(u32));
                    out.flush();
                    out.close();
                }
            }
        }
        return outBinaries;
    }

    SPIRVBinaryDatas ShaderCompiler::CompileOpenGLBinaries(const ShaderPreprocessResult& processResult)
    {
        auto vulkanBinaryDatas = CompileVulkanBinaries(processResult);
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        constexpr bool optimize = false;
        if (optimize)
            { options.SetOptimizationLevel(shaderc_optimization_level_performance);}
        File::Path cacheDir = Utils::Shader::GetShaderCacheDir();

        SPIRVBinaryDatas outBinaries;
        Map<ShaderStage, String>GLSLSources;

        for (auto&& [stage, binary] : vulkanBinaryDatas)
        {
            File::Path cachedPath = cacheDir / (processResult.Name + Utils::Shader::ShaderStageCachedOpenGLFileExtension(stage));
            std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0,std::ios::end);
                const size_t size = in.tellg();
                in.seekg(0,std::ios::beg);

                auto& data = outBinaries[stage];
                data.resize(size / sizeof(u32));
                in.read(reinterpret_cast<char*>(data.data()), size);
            }
            else
            {
                spirv_cross::CompilerGLSL glslCompiler(binary);
                GLSLSources[stage] = glslCompiler.compile();
                auto& source = GLSLSources[stage];

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::Shader::GetShaderKind(stage), processResult.Name.c_str());
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    PL_ASSERT(false, "Shader Compilation Error: " + module.GetErrorMessage());
                    return {};
                }

                outBinaries[stage] = {module.cbegin(), module.cend()};

                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& binaries = outBinaries[stage];
                    out.write(reinterpret_cast<char*>(binaries.data()), binaries.size() * sizeof(u32));
                    out.flush();
                    out.close();
                }
            }
        }
        return outBinaries;

    }

    ShaderPreprocessResult ShaderCompiler::PreprocessShaderFile(const File::Path& filePath)
    {
        ShaderPreprocessResult result;
        result.Success = false;
        // 获取文件扩展名
        const String ext = filePath.extension().string();
        if (ext == ".hlsl") {result.SourceLang = Utils::Shader::SourceLang::HLSL;}
        else if (ext == ".glsl") {result.SourceLang = Utils::Shader::SourceLang::GLSL;}
        else
        {
            result.ErrorMessage = "Unknown shader file extension!" + ext;
            return result;
        }
        // 获取文件名做为shader名称
        result.Name = filePath.stem().string();

        const String shaderSource = ReadFile(filePath);
        if (shaderSource.empty())
        {
            result.ErrorMessage = "Could not read file!";
            return result;
        }

        Map <ShaderStage, String> sources;
        StringView typeToken = "#pragma type ";
        size_t typeTokenLength = typeToken.length();
        size_t pos = shaderSource.find(typeToken, 0);

        while (pos != String::npos)
        {
            // 提取 Shader 类型
            size_t typeStart = pos + typeTokenLength;
            size_t typeEnd = shaderSource.find_first_of("\r\n", typeStart);
            if (typeEnd == String::npos) {
                result.ErrorMessage = "Invalid #pragma type directive!";
                return result;
            }
            // 获取类型字符串
            String typeStr = shaderSource.substr(typeStart, typeEnd - typeStart);
            PString::StringTrim(typeStr); // 去除前后空白字符
            // 将类型字符串转换为 ShaderStage
            ShaderStage stage = Utils::Shader::ShaderTypeFromString(typeStr);
            if (stage == ShaderStage::None) {
                result.ErrorMessage = "Unknown shader stage: " + typeStr;
                return result;
            }
            // 提取当前阶段的代码
            size_t codeStart = typeEnd + 1; // 跳过换行符
            size_t codeEnd = shaderSource.find(typeToken, codeStart);
            if (codeEnd == String::npos) {
                codeEnd = shaderSource.length(); // 如果是最后一个阶段，代码到文件末尾
            }
            // 保存当前阶段的代码
            sources[stage] = shaderSource.substr(codeStart, codeEnd - codeStart);;
            // 更新搜索位置
            pos = shaderSource.find(typeToken, codeEnd);

        }

        if (sources.empty()) {
            result.ErrorMessage = "No valid shader stages found";
            return result;
        }

        result.Sources = sources;
        result.Success = true;
        return result;

    }
    String ShaderCompiler::ReadFile(const File::Path& filePath)
    {
        Log::CatInfo("Shader","Shader Compiler Reading File Path : {0}",filePath.string());
        String result;
        if (std::ifstream in(filePath, std::ios::in | std::ios::binary); in)
        {
            in.seekg(0, std::ios::end);
            if (const size_t size = in.tellg(); size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
                in.close();
            }
            else
            {
                Log::CatError("Shader", "Could not read from file '{0}'", filePath.string());
            }
        }
        else
        {
            Log::CatError("Shader","Could not read from file '{0}'", filePath.string());
        }
        return result;
    }
}
