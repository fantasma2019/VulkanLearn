#pragma once

#include <shaderc/shaderc.hpp>

#include "ShaderUtils.h"
#include "Core/BaseType.h"
#include "Core/FileSystem.h"


// [Shader Asset] -> [Preprocessor] -> [IR Generator] ->[API Compiler]->[Bytecode]->[Cache System]->[Runtime Loader]


using SPIRVBinary = Vector<u32>;
using SPIRVBinaryDatas = Map<ShaderStage, SPIRVBinary>;

namespace ShaderAPIFlags
{
    enum ShaderAPIFlags : u8
    {
        None = 0,
        Vulkan  = BIT(0),
        OpenGL  = BIT(1),
        DirectX = BIT(2),
    };
}

/**
 * @brief Shader编译错误级别
 * @note
 * Error  等级会导致程序无法运行 \n
 * Warning等级会导致程序运行，但可能会有问题 \n
 * Normal 等级只是提示信息
 */
enum class ShaderCompileErrorLevel : u8
{
    Normal,
    Warning,
    Error
};

// 编译结果
struct CacheCompileResult
{
    String ShaderName;
    SPIRVBinaryDatas Sources;
    bool Success {false};
    String ErrorMessage;
};

// SPIRV编译结果
struct SPIRVCompilerResult
{
    Map<ShaderStage, SPIRVBinary> SPIRVs; // SPIRV二进制数据
    String ErrorMessage;                  // 错误信息
    bool Success {false};                 // 是否成功
};
// Shader预处理结果
struct ShaderPreprocessResult
{
    String Name;                          // Shader名称
    ShaderSourceLang SourceLang;   // Shader语言
    Map<ShaderStage, String> Sources;     // Shader源码
    bool Success = false;                 // 是否成功
    String ErrorMessage;                  // 错误信息
};

namespace Utils::Shader
{
    static Str ShaderStageCachedFileExtension(const ShaderStage stage, const u8 flags)
    {
        if(flags & ShaderAPIFlags::Vulkan)
        {
            return ShaderStageCachedVulkanFileExtension(stage);
        }
        if(flags & ShaderAPIFlags::OpenGL)
        {
            return ShaderStageCachedOpenGLFileExtension(stage);
        }
        if(flags & ShaderAPIFlags::DirectX)
        {
            return ShaderStageCachedDirectXFileExtension(stage);
        }
        PL_ASSERT(false,"No Support ShaderAPI Type!");
        return "";
    }
    static bool CheckShaderPreprocessResult(const ShaderPreprocessResult& result)
    {
        if (!result.Success || result.Sources.empty() ||
            result.SourceLang == ShaderSourceLang::None ||
            result.SourceLang == ShaderSourceLang::Hlsl
            )
        {
            return false;
        }

        return true;
    }

}

/**
 * @brief Shader编译器
 */
class ShaderCompiler
{
public:
    // 预处理文件
    static ShaderPreprocessResult PreprocessShaderFile(const File::Path& filePath);

    static CacheCompileResult CacheCompile(const File::Path& filePath, u8 flags = ShaderAPIFlags::OpenGL);

    // 编译Vulkan SPIRV
    static SPIRVBinaryDatas CompileVulkanBinaries(const ShaderPreprocessResult& processResult);

    // 编译OpenGL SPIRV，将Vulkan SPIRV转换为OpenGL SPIRV以消除Vulkan标准的GLSL差异
    static SPIRVBinaryDatas CompileOpenGLBinaries(const ShaderPreprocessResult& processResult);

    static String ReadFile(const File::Path& filePath);
};
