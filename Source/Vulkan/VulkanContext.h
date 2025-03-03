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

    SharedPtr<VkSurfaceKHR> CreateSurface(GLFWwindow* window);
    SharedPtr<VkPhysicalDevice> SelectPhysicalDevice();
    SharedPtr<VkDevice> CreateVkDevice();


    static VkInstance GetInstance() { return s_VulkanInstance;}
    SharedPtr<VkPhysicalDevice> GetPhysicalDevice() { return m_PhysicalDevice;}
    SharedPtr<VkDevice>GetDevice() { return m_Device;}
    SharedPtr<VkSurfaceKHR> GetSurface() { return m_Surface;}

    static void Init();

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
    inline static VkInstance s_VulkanInstance;

    DynamicArray<Str> m_InstanceLayers;
    DynamicArray<Str> m_InstanceExtensions;


};
