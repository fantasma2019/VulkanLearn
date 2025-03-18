#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#if defined(USE_VOLK)
    #define VOLK_IMPLEMENTATION
    #include <Volk/volk.h>
#else
    #include <vulkan/vulkan.h>
    #define VK_EXPORTED_FUNCTION(func) extern PFN_##func func;
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/**
 * RenderSystem          渲染系统
 *
 * VulkanRenderer        渲染后端
 *
 * VulkanContext         Vulkan上下文，Vk实例和设备管理
 * VulkanSwapChain       交换链
 * VulkanRenderPass      渲染通道
 * VulkanPipeline        图像管线
 * VulkanCommandBuffer   命令缓冲区
 * VulkanBuffer          缓冲区
 * VulkanTexture         纹理和图像
 * VulkanShader          着色器
 * VulkanDescriptor      描述符
 * VulkanSync            同步原语
 */
