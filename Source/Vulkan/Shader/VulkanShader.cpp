#include "VulkanShader.h"

Shader::Shader(const File::Path& path)
{
    const auto extension = path.extension().string();

    if (extension == ".glsl")
    {
        // 先编译成二进制

        // 再读取
    }
    else if (extension == ".hlsl")
    {
        // 先编译成二进制

        // 再读取
    }
    else if (extension == ".spv")
    {
        // 读取
    }
    else
    {
        throw std::runtime_error("Unsupported shader file extension");
    }
}

Shader::Shader(const String& code)
{
}

Shader::~Shader()
{
}

void ShaderLibrary::Add(const SharedPtr<Shader>& shader)
{
}

SharedPtr<Shader> ShaderLibrary::Load(const File::Path& path)
{
    return MakeShared<Shader>(path);
}

SharedPtr<Shader> ShaderLibrary::Get(const String& name)
{
    return nullptr;
}

bool ShaderLibrary::IsExists(const String& name) const
{
    return false;
}

void ShaderLibrary::Remove(const String& name)
{
}
