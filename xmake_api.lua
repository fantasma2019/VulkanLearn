---@diagnostic disable: undefined-global

-- xmake 类型定义
---@class XmakeTarget
---@field set_kind fun(kind: string)
---@field add_files fun(files: string)
---@field add_includedirs fun(dirs: string)
---@field add_packages fun(packages: string)
---@field add_deps fun(deps: string)

-- 全局函数
---@param name string 项目名称
---@return nil
function set_project(name) end

---@param version string 版本号
---@return nil
function set_version(version) end

---@param ... string 构建模式
---@return nil
function add_rules(...) end

---@param lang string 语言标准
---@return nil
function set_languages(lang) end

---@param pchPath string 预编译头文件路径
---@return nil
function set_pcxxheader(pchPath) end

---@param package string 包名称
---@return nil
function add_requires(package) end

---@param toolchain string 工具链名称
---@return nil
function add_toolchains(toolchain) end

---@param includedirs string include文件路径
---@return nil
function add_includedirs(includedirs)  end

---@param headerfiles string 头文件路径
---@return nil
function add_headerfiles(headerfiles)  end

---@param depslist string 依赖库
---@return nil
function add_deps(depslist)  end

---@param targetname string 目标名称
---@return XmakeTarget
function target(targetname) end
