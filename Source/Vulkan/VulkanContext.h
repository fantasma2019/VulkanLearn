#pragma once
#include "BaseType.h"
#include "Vulkan.h"

/**
 * @brief Vulkan上下文
 * @details
 * 管理Vulkan实例、物理设备选择和逻辑设备创建 \n
 * 处理验证层和扩展
 */
class VulkanContext
{
public:
    static VulkanContext& Get()
    {
        static VulkanContext instance;
        return instance;
    }

    SharedPtr<VkInstance> CreateInstance();
    SharedPtr<VkSurfaceKHR> CreateSurface(GLFWwindow* window);
    SharedPtr<VkPhysicalDevice> SelectPhysicalDevice();
    SharedPtr<VkDevice> CreateVkDevice();


    SharedPtr<VkInstance> GetInstance() { return s_VulkanInstance;}
    SharedPtr<VkPhysicalDevice> GetPhysicalDevice() { return m_PhysicalDevice;}
    SharedPtr<VkDevice>GetDevice() { return m_Device;}
    SharedPtr<VkSurfaceKHR> GetSurface() { return m_Surface;}

    void Init();

    VulkanContext();
    ~VulkanContext();
    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

private:
    bool CheckValidationLayerSupport();
    DynamicArray<Str> GetRequiredExtensions();
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    bool IsDeviceSuitable(VkPhysicalDevice device);

private:
    SharedPtr<VkPhysicalDevice> m_PhysicalDevice;
    SharedPtr<VkDevice> m_Device;
    SharedPtr<VkSurfaceKHR> m_Surface;
    SharedPtr<VkInstance> s_VulkanInstance = VK_NULL_HANDLE;

    DynamicArray<Str> m_InstanceLayers;
    DynamicArray<Str> m_InstanceExtensions;

    const DynamicArray<Str> m_ValidationLayers {"VK_LAYER_KHRONOS_validation"};
    const DynamicArray<Str> m_DeviceExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
    const bool m_EnableValidationLayers = false;
#else
    const bool m_EnableValidationLayers = true;
#endif
};
