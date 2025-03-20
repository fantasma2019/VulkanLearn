#include "VulkanShader.h"

Shader::Shader(File::Path path)
{

    std::filesystem::

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
}

SharedPtr<Shader> ShaderLibrary::Get(const String& name)
{
}

bool ShaderLibrary::IsExists(const String& name) const
{
}

void ShaderLibrary::Remove(const String& name)
{
}
