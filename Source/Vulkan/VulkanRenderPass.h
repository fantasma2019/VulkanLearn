#pragma once
#include "VulkanContext.h"

namespace Vulkan {

    class VulkanRenderPass {
    public:
        VulkanRenderPass(VulkanContext* context);
        ~VulkanRenderPass();

        // 禁止拷贝
        VulkanRenderPass(const VulkanRenderPass&) = delete;
        VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;

        // 创建渲染通道
        void Create(VkFormat swapChainImageFormat);

        // 获取渲染通道
        VkRenderPass GetRenderPass() const { return renderPass; }

    private:
        VulkanContext* context;
        VkRenderPass renderPass;
    };

} // namespace Vulkan
