#pragma once
#include "VulkanContext.h"

namespace Vulkan {

    class VulkanSwapChain {
    public:
        VulkanSwapChain(VulkanContext* context);
        ~VulkanSwapChain();

        // 禁止拷贝
        VulkanSwapChain(const VulkanSwapChain&) = delete;
        VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;

        // 创建交换链
        void Create(GLFWwindow* window);

        // 重建交换链
        void Recreate(GLFWwindow* window);

        // 清理交换链
        void Cleanup();

        // 获取下一个图像
        VkResult AcquireNextImage(uint32_t* imageIndex, VkSemaphore signalSemaphore);

        // 获取当前图像索引
        VkResult GetCurrentImageIndex(uint32_t* imageIndex);

        // 显示图像
        VkResult PresentImage(uint32_t imageIndex, VkSemaphore* waitSemaphores, uint32_t waitSemaphoreCount);

        // 获取访问器
        VkFormat GetImageFormat() const { return imageFormat; }
        VkExtent2D GetExtent() const { return extent; }
        uint32_t GetImageCount() const { return static_cast<uint32_t>(images.size()); }
        const std::vector<VkImageView>& GetImageViews() const { return imageViews; }

    private:
        // 选择交换链表面格式
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        // 选择交换链显示模式
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        // 选择交换链尺寸
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

        // 创建图像视图
        void CreateImageViews();

    private:
        VulkanContext* context;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        VkFormat imageFormat;
        VkExtent2D extent;

        uint32_t currentImageIndex;

        GLFWwindow* currentWindow;
    };

} // namespace Vulkan
