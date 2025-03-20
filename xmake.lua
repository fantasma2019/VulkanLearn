add_rules("mode.debug", "mode.release")
add_rules ("plugin.vsxmake.autoupdate")
set_project("VulkanRenderer")

set_languages("c++20")
set_toolchains("msvc")

local projectdir = os.projectdir()
local vulkan_sdk_path = os.getenv("VULKAN_SDK") or "Engine/Source/ThirdParty/VulkanSDK"

if is_mode("debug") then
    add_defines("DEBUG", "PL_DEBUG")

elseif is_mode("release") then
    add_defines("RELEASE", "PL_RELEASE","NDEBUG")
end

if is_plat("windows") then
    add_defines("WINDOWS")
    add_defines("PL_PLAT_WINDOWS")
    projectdir = projectdir:gsub("\\", "/")
    add_defines("PL_WORK_DIR=\"" .. projectdir .. "\"")

elseif is_plat("linux") then
    add_defines("PL_PLAT_LINUX")
    add_defines("PL_WORK_DIR=\"" .. projectdir .. "\"")

elseif is_plat("macosx") then
    add_defines("PL_PLAT_MACOSX")
    add_defines("PL_WORK_DIR=\"" .. projectdir .. "\"")
end
--lib--
add_requires("spdlog >= 1.15.0", "glm", "glfw >= 3.3.8")

local includedirs =
{
    "Source/ThirdParty/VulkanSDK/include",
    "Source/ThirdParty",
}

local Deps = { "spdlog", "glm", "glfw", }

target("VulkanRenderer")
    set_kind("binary")
    add_includedirs(includedirs)
    add_packages(Deps)

    set_pcxxheader("Source/Vulkan/vkpch.h")

    add_headerfiles("Source/Vulkan/**.h")
    add_files("Source/Vulkan/**.cpp")
    add_files("Source/*.cpp")

if is_plat("windows") then
    add_defines("VK_USE_PLATFORM_WIN32_KHR")
    add_linkdirs("Source/ThirdParty/VulkanSDK/Lib")
    add_links("vulkan-1")
elseif is_plat("linux") then
    add_defines("VK_USE_PLATFORM_XCB_KHR")
    add_linkdirs("/usr/lib")
    add_links("vulkan")
elseif is_plat("macosx") then
    add_defines("VK_USE_PLATFORM_METAL_EXT")
    add_frameworks("Metal", "Foundation", "Cocoa", "QuartzCore")
    add_linkdirs("/usr/local/lib")
    add_links("vulkan")
end

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--
