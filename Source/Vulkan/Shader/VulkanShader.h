#pragma once
#include "../VulkanUtils.h"
#include "../Core/BaseType.h"
#include "../Core/FileSystem.h"



class Shader
{
public:
    Shader(const File::Path& path);
    Shader(const String& code);
    ~Shader();
};

class ShaderLibrary
{
    using ShaderMap = Map<String, SharedPtr<Shader>>;
public:
    void Add(const SharedPtr<Shader>& shader);
    SharedPtr<Shader> Load(const File::Path& path);
    SharedPtr<Shader> Get(const String& name);

    bool IsExists(const String& name) const ;
    void Remove(const String& name);

private:
    ShaderMap m_Shaders;
};

class Material
{

};
