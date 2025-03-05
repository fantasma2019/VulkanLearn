#pragma once
#include "Vulkan.h"

struct GLFWwindow;


/**
 * @brief Vulkan 高级渲染API
 * @code
 * // 使用用例
 * Renderer::BeginFrame();
 * Renderer::Submit(Mesh);
 * Renderer::EndFrame();
 * @endcode
 */
class VulkanRenderer
{
public:
    VulkanRenderer();
    ~VulkanRenderer();

};
