#pragma once
#include "BaseType.h"
#include "VulkanContext.h"

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
        VkResult AcquireNextImage(u32* imageIndex, VkSemaphore signalSemaphore);

        // 获取当前图像索引
        VkResult GetCurrentImageIndex(u32* imageIndex);

        // 显示图像
        VkResult PresentImage(u32 imageIndex, VkSemaphore* waitSemaphores, u32 waitSemaphoreCount);

        // 获取访问器
        VkFormat GetImageFormat() const { return imageFormat; }
        VkExtent2D GetExtent() const { return extent; }
        u32 GetImageCount() const { return static_cast<u32>(images.size()); }
        const DynamicArray<VkImageView>& GetImageViews() const { return imageViews; }

    private:
        // 选择交换链表面格式
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);

        // 选择交换链显示模式
        VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes);

        // 选择交换链尺寸
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

        // 创建图像视图
        void CreateImageViews();

    private:
        VulkanContext* context;

        VkSwapchainKHR swapChain;
        DynamicArray<VkImage> images;
        DynamicArray<VkImageView> imageViews;
        VkFormat imageFormat;
        VkExtent2D extent;

        u32 currentImageIndex;

        GLFWwindow* currentWindow;
    };
