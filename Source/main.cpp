#include <iostream>

#include "Vulkan/VulkanWindow.h"
#undef NDEBUG

int main(int argc, char** argv)
{
    std::cout << "hello world!" << std::endl;
    auto window = Window::Create();
    window->Init();

    while (!window->ShouldClose())
    {
        window->OnUpdate();
    }

    return 0;
}
