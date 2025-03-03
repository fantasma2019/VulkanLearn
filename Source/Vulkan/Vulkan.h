#pragma once
#include <vulkan/vulkan.h>
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
