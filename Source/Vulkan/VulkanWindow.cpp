#include "vkpch.h"
#include "VulkanWindow.h"

#include "BaseType.h"

Window::Window(const WindowInitData& initData)
    : m_InitData(initData)
{
}

Window::~Window()
{
    Shutdown();
}

UniquePtr<Window> Window::Create(const WindowInitData& initData)
{
    return std::make_unique<Window>(initData);
}

void Window::DisFPSToTitle()
{
    static auto lastTime = glfwGetTime();
    static double time;
    static double timeStep;
    static u_int cumFrame = -1;
    time = glfwGetTime();
    cumFrame++;
    if ((timeStep = time - lastTime) >= 1.0)
    {
        glfwSetWindowTitle(m_Window, (std::string(m_InitData.Title) + " - FPS: " + std::to_string((int)(cumFrame / timeStep))).c_str());
        lastTime = time;
        cumFrame = 0;
    }
}

void Window::OnUpdate()
{
    glfwPollEvents();
    DisFPSToTitle();
}

void Window::Init()
{
    if (!glfwInit())
        PL_ASSERT(false, "Failed to init GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_Window = glfwCreateWindow(m_InitData.Width, m_InitData.Height, m_InitData.Title.data(), nullptr, nullptr);
    m_Monitor= glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);

    if (!m_Window)
    {
        glfwTerminate();
        PL_ASSERT(false, "Failed to create window");
    }

}

void Window::Shutdown()
{
    glfwTerminate();
}
