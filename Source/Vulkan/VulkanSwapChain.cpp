#include "VulkanSwapChain.h"
#include "VulkanUtils.h"
#include <algorithm>

VulkanSwapChain::VulkanSwapChain(VulkanContext* context)
    : context(context)
    , swapChain(VK_NULL_HANDLE)
    , currentImageIndex(0)
    , currentWindow(nullptr)
{
}

VulkanSwapChain::~VulkanSwapChain() {
    Cleanup();
}

void VulkanSwapChain::Create(GLFWwindow* window) {
    currentWindow = window;

    // 查询交换链支持
    SwapChainSupportDetails swapChainSupport = context->QuerySwapChainSupportDetails(context->GetPhysicalDevice());

    // 选择表面格式、显示模式和交换范围
    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, window);

    // 确定图像数量
    u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    // 创建交换链信息
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = context->GetSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // 处理不同队列族
    QueueFamilyIndices indices = context->FindQueueFamilies(context->GetPhysicalDevice());
    u32 queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // 创建交换链
    VK_CHECK(vkCreateSwapchainKHR(context->GetDevice(), &createInfo, nullptr, &swapChain));

    // 获取交换链图像
    vkGetSwapchainImagesKHR(context->GetDevice(), swapChain, &imageCount, nullptr);
    images.resize(imageCount);
    vkGetSwapchainImagesKHR(context->GetDevice(), swapChain, &imageCount, images.data());

    // 保存格式和范围
    imageFormat = surfaceFormat.format;
    this->extent = extent;

    // 创建图像视图
    CreateImageViews();
}

void VulkanSwapChain::Recreate(GLFWwindow* window) {
    // 等待设备空闲
    vkDeviceWaitIdle(context->GetDevice());

    // 清理旧资源
    Cleanup();

    // 创建新交换链
    Create(window);
}

void VulkanSwapChain::Cleanup() {
    // 销毁图像视图
    for (auto imageView : imageViews) {
        vkDestroyImageView(context->GetDevice(), imageView, nullptr);
    }

    // 销毁交换链
    if (swapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(context->GetDevice(), swapChain, nullptr);
        swapChain = VK_NULL_HANDLE;
    }

    images.clear();
    imageViews.clear();
}

VkResult VulkanSwapChain::AcquireNextImage(u32* imageIndex, VkSemaphore signalSemaphore) {
    VkResult result = vkAcquireNextImageKHR(
        context->GetDevice(),
        swapChain,
        UINT64_MAX,
        signalSemaphore,
        VK_NULL_HANDLE,
        imageIndex
    );

    if (result == VK_SUCCESS) {
        currentImageIndex = *imageIndex;
    }

    return result;
}

VkResult VulkanSwapChain::GetCurrentImageIndex(u32* imageIndex) {
    *imageIndex = currentImageIndex;
    return VK_SUCCESS;
}

VkResult VulkanSwapChain::PresentImage(u32 imageIndex, VkSemaphore* waitSemaphores, u32 waitSemaphoreCount) {
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = waitSemaphoreCount;
    presentInfo.pWaitSemaphores = waitSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    return vkQueuePresentKHR(context->GetPresentQueue(), &presentInfo);
}

VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats) {
    // 寻找首选格式
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // 如果找不到首选格式，使用第一个可用格式
    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes) {
    // 寻找首选模式
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    // 备用模式 - FIFO总是被支持
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
    // 如果当前尺寸已设置
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        // 否则使用窗口尺寸
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<u32>(width),
            static_cast<u32>(height)
        };

#ifdef max
#undef max
#undef min
#endif
        // 限制在最大和最小范围内
        actualExtent.width = std::max(capabilities.minImageExtent.width,
                             std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                              std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

void VulkanSwapChain::CreateImageViews() {
    imageViews.resize(images.size());

    for (size_t i = 0; i < images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = imageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(context->GetDevice(), &createInfo, nullptr, &imageViews[i]));
    }
}
