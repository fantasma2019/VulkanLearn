#pragma once
#include "Vulkan.h"

struct WindowInitData
{
    std::string_view Title  = "VulkanRenderer";
    u_int Width       = 1280;
    u_int Height      = 720;
    bool Decorated    = true;
    bool FullScreen   = false;
    bool VSync        = true;
    bool CanResize    = true;
    std::filesystem::path IconPath;
};


class Window
{
public:
    Window(const WindowInitData& initData);
    ~Window();
    bool ShouldClose() {return glfwWindowShouldClose(m_Window);}

    static std::unique_ptr<Window> Create(const WindowInitData& initData = WindowInitData{});

    void DisFPSToTitle();
    void OnUpdate();
    void Init();

    GLFWwindow* GetWindow() const {return m_Window;}

private:
    void Shutdown();
private:
    GLFWwindow* m_Window;
    GLFWcursor* m_Cursor[9] {0};
    GLFWmonitor* m_Monitor;
    WindowInitData m_InitData;
};
