#include "vkpch.h"
#include "VulkanContext.h"

VulkanContext::VulkanContext()
    : m_PhysicalDevice(VK_NULL_HANDLE), m_Device(VK_NULL_HANDLE), m_Surface(VK_NULL_HANDLE)
{
}
VulkanContext::~VulkanContext()
{
    if (m_Device != VK_NULL_HANDLE)
        vkDestroyDevice(m_Device, nullptr);
    if (m_Surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(s_VulkanInstance,m_Surface,nullptr);
    if (s_VulkanInstance != VK_NULL_HANDLE)
        vkDestroyInstance(s_VulkanInstance, nullptr);
}

void VulkanContext::CreateInstance()
{
    // 检查验证层
    if (m_EnableValidationLayers && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    // 应用信息
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "VulkanRenderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName        = "No Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_2;

    // Instance创建信息
    VkInstanceCreateInfo createInfo{};
    createInfo.sType               = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo    = &appInfo;

    // 拓展
    const auto extension = GetRequiredExtensions();
    createInfo.enabledExtensionCount      = static_cast<uint32_t>(extension.size());
    createInfo.ppEnabledExtensionNames    = extension.data();

    // 验证层
    if (m_EnableValidationLayers)
    {
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_ValidationLayers.size());
        createInfo.ppEnabledLayerNames   = m_ValidationLayers.data();
    }
    else
        createInfo.enabledExtensionCount = 0;

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &s_VulkanInstance));

}

void VulkanContext::CreateSurface(GLFWwindow* window)
{
    VK_CHECK(glfwCreateWindowSurface(s_VulkanInstance, window, nullptr, &m_Surface));
}


void VulkanContext::SelectPhysicalDevice()
{
    u32 deviceCount {0};
    vkEnumeratePhysicalDevices(s_VulkanInstance, &deviceCount, nullptr);
    if (deviceCount == 0)
        throw std::runtime_error("failed to find GPUs with Vulkan support!");

    DynamicArray<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(s_VulkanInstance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (IsDeviceSuitable(device))
        {
            m_PhysicalDevice = device;
            break;
        }
    }

    if (!m_PhysicalDevice)
        throw std::runtime_error("failed to find a suitable GPU!");
}

void VulkanContext::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    DynamicArray<VkDeviceQueueCreateInfo> queueCreateInfos;
    Set<u32> uniqueQueueFamilies {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };
    float queuePriority {1.0f};
    for (u32 quequeFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = quequeFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // 设备功能
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos       = queueCreateInfos.data();
    createInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures        = &deviceFeatures;
    createInfo.enabledExtensionCount   = static_cast<uint32_t>(m_DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

    if (m_EnableValidationLayers)
    {
        createInfo.enabledLayerCount   = static_cast<uint32_t>(m_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount   = 0;
    }

    VK_CHECK(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device));

    // 获取队列句柄
    vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);

}

QueueFamilyIndices VulkanContext::FindQueueFamilies(VkPhysicalDevice device) const
{
    QueueFamilyIndices indices;

    u32 queueFamilyCount {0};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // 查找支持图像的队列组族
    for (u32 i {0}; i < queueFamilyCount; ++i)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
        if (presentSupport)
            indices.presentFamily = i;

        if (indices.IsComplete())
            break;
    }

    return indices;
}

SwapChainSupportDetails VulkanContext::QuerySwapChainSupportDetails(VkPhysicalDevice device) const
{
    SwapChainSupportDetails details;

    // 查询基本表面功能
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

    // 查询表面格式
    u32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.formats.data());
    }

    // 查询支持的交换链模式
    u32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool VulkanContext::CheckValidationLayerSupport()
{
    u32 layoutCount;
    vkEnumerateInstanceLayerProperties(&layoutCount, nullptr);
    DynamicArray<VkLayerProperties> availableLayers(layoutCount);
    vkEnumerateInstanceLayerProperties(&layoutCount, availableLayers.data());

    // 检查所有请求的验证层是否可用
    for (Str layerName :  m_ValidationLayers)
    {
        bool isLayerFound {false};
        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                isLayerFound = true;
                break;
            }
        }
        if (!isLayerFound)
            return false;
    }
    return true;
}

DynamicArray<Str> VulkanContext::GetRequiredExtensions() const
{
    u32 glfwExtensionCount {0};
    Str* glfwExtensions {glfwGetRequiredInstanceExtensions(&glfwExtensionCount)};
    DynamicArray<Str> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (m_EnableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

bool VulkanContext::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    u32 extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    DynamicArray<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    Set<String> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());
    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices {FindQueueFamilies(device)};

    bool isExtensionsSupported {CheckDeviceExtensionSupport(device)};

    bool isSwapChainAdequate {false};

    if (isExtensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport {QuerySwapChainSupportDetails(device)};
        isSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.IsComplete() && isExtensionsSupported && isSwapChainAdequate;
}

/**
 * @brief VulkanContext初始化
 * @details
 * 初始化流程:
 * 1. 创建层(Layer)和拓展(Extension)来提供对特定需求和平台的支持 \n
 * 2. 创建实例(VkInstance)来显式的提供Vulkan所需内容给系统 \n
 * 3. 创建调试信息(VkDebugUtilsMessengerEXT)来获取验证层捕获的debug信息 \n
 * 4. 创建Surface(VkSurfaceKHR)来和平台特定的窗口对接 \n
 * 5. 创建物理设备(VkPhysicalDevice)来获取硬件信息 \n
 * 6. 创建逻辑设备(VkDevice)来管理物理设备 \n
 * 7. 创建队列(VkQueue)来进行命令的提交和执行 \n
 * 8. 创建交换链(VkSwapchainKHR)来管理帧缓冲区(VkFramebuffer)和图像(VkImage) \n
 */
void VulkanContext::Init()
{

}
