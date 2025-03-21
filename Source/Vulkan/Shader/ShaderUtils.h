#pragma once
#include "VulkanUtils.h"
#include "Core/BaseType.h"

/** @brief Shader的阶段种类*/
enum class ShaderStage
{
    None = 0,
    Vertex,           ///< 顶点阶段
    Fragment,         ///< 片段阶段
    Compute,          ///< 计算阶段
    Geometry,         ///< 几何阶段
    TessControl,      ///< 控制阶段
    TessEvaluation    ///< 评估阶段
};


/** @brief Shader源语言 */
enum class ShaderSourceLang
{
    None = 0, Glsl, Hlsl, Spv
};

/** @brief Shader uniform类型*/
enum class ShaderUniformType
{
    None = 0, Bool, Int, UInt, Float, Vec2, Vec3, Vec4, Mat3, Mat4, IVec2, IVec3, IVec4, Sampler2D
};

namespace Utils::Shader
{
    static ShaderStage ShaderTypeFromString(const String& type)
    {
        if (type == "vertex")
            return ShaderStage::Vertex;
        if (type == "fragment" || type == "pixel")
            return ShaderStage::Fragment;
        if (type == "compute")
            return ShaderStage::Compute;
        if (type == "geometry")
            return ShaderStage::Geometry;
        PL_ASSERT(false, "Unknown shader type!");
        return ShaderStage::None;
    }
    static Str ShaderStageToString(ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::Vertex:           return "Vertex";
        case ShaderStage::Fragment:         return "Fragment";
        case ShaderStage::Compute:          return "Compute";
        case ShaderStage::Geometry:         return "Geometry";
        case ShaderStage::TessControl:      return "TessControl";
        case ShaderStage::TessEvaluation:   return "TessEvaluation";
        default: return "Unknown";
        }
    }

    static ShaderSourceLang ShaderSourceLangFromString(const String& lang)
    {
        if (lang == "glsl")
            return ShaderSourceLang::Glsl;
        if (lang == "hlsl")
            return ShaderSourceLang::Hlsl;
        if (lang == "spv")
            return ShaderSourceLang::Spv;
        PL_ASSERT(false, "Unknown shader source language!");
        return ShaderSourceLang::None;
    }
    static Str ShaderSourceLangToString(ShaderSourceLang lang)
    {
        switch (lang)
        {
        case ShaderSourceLang::Glsl:    return "Glsl";
        case ShaderSourceLang::Hlsl:    return "Hlsl";
        case ShaderSourceLang::Spv:     return "Spv";
        default: return "Unknown";
        }
    }
    static String FileExtensionFromShaderLang(ShaderSourceLang lang)
    {
        switch (lang)
        {
        case ShaderSourceLang::Glsl:    return ".glsl";
        case ShaderSourceLang::Hlsl:    return ".hlsl";
        case ShaderSourceLang::Spv:     return ".spv";
        default: return "";
        }
    }

    // 获取shader缓存目录
    static Str GetShaderCacheDir() { return ".cache/asset/shader";}
    // 获取shader生成目录
    static Str GetShaderGenerateDir() { return "asset/shader/generate";}

    // 初始化shader缓存目录
    static void InitShaderCacheDir()
    {
        if (const String cacheDir = GetShaderCacheDir(); !std::filesystem::exists(cacheDir))
        {
            std::filesystem::create_directories(cacheDir);
        }
    }
    // 初始化shader生成目录
    static void InitShaderGenerateDir()
    {
        if (const String generateDir = GetShaderGenerateDir(); !std::filesystem::exists(generateDir))
        {
            std::filesystem::create_directories(generateDir);
        }
    }
    // 获取shader类型
    static shaderc_shader_kind GetShaderKind(const ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::Vertex:           return shaderc_vertex_shader;
        case ShaderStage::Fragment:         return shaderc_fragment_shader;
        case ShaderStage::Compute:          return shaderc_compute_shader;
        case ShaderStage::Geometry:         return shaderc_geometry_shader;
        case ShaderStage::TessControl:      return shaderc_tess_control_shader;
        case ShaderStage::TessEvaluation:   return shaderc_tess_evaluation_shader;
        default:                            return shaderc_glsl_infer_from_source;
        }
    }
    // 获取OpenGL Cache后缀字符串
    static Str ShaderStageCachedOpenGLFileExtension(const ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::Vertex:   return ".cached_gl.vert";
        case ShaderStage::Fragment: return ".cached_gl.frag";
        case ShaderStage::Compute:  return ".cached_gl.comp";
        case ShaderStage::Geometry: return ".cached_gl.geom";
        default: PL_ASSERT(false,"No Support ShaderStage Type!");return "";
        }
    }
    // 获取vulkan Cache后缀字符串
    static Str ShaderStageCachedVulkanFileExtension(const ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::Vertex:   return ".cached_vk.vert";
        case ShaderStage::Fragment: return ".cached_vk.frag";
        case ShaderStage::Compute:  return ".cached_vk.comp";
        case ShaderStage::Geometry: return ".cached_vk.geom";
        default: PL_ASSERT(false,"No Support ShaderStage Type!");return "";
        }
    }
    //@TODO: 不一定正确，还没实现DX相关
    static Str ShaderStageCachedDirectXFileExtension(const ShaderStage stage)
    {
        switch (stage)
        {
        case ShaderStage::Vertex:   return ".cached_dx.vert";
        case ShaderStage::Fragment: return ".cached_dx.frag";
        case ShaderStage::Compute:  return ".cached_dx.comp";
        case ShaderStage::Geometry: return ".cached_dx.geom";
        default: PL_ASSERT(false,"No Support ShaderStage Type!");return "";
        }
    }

}
