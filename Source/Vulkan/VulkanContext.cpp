#include "vkpch.h"
#include "VulkanContext.h"

VulkanContext::VulkanContext()
    : m_PhysicalDevice(VK_NULL_HANDLE), m_Device(VK_NULL_HANDLE), m_Surface(VK_NULL_HANDLE)
{
}
VulkanContext::~VulkanContext()
{
    if (m_Device != VK_NULL_HANDLE)
        vkDestroyDevice(*m_Device, nullptr);
    if (m_Surface != VK_NULL_HANDLE)
        vkDestroySurfaceKHR(s_VulkanInstance,*m_Surface,nullptr);
    if (s_VulkanInstance != VK_NULL_HANDLE)
        vkDestroyInstance(s_VulkanInstance, nullptr);
}

SharedPtr<VkInstance> VulkanContext::CreateInstance()
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

    // 创建信息
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

    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &*s_VulkanInstance));


}

SharedPtr<VkDevice> VulkanContext::CreateVkDevice()
{
}



SharedPtr<VkSurfaceKHR> VulkanContext::CreateSurface(GLFWwindow* window)
{
}

SharedPtr<VkPhysicalDevice> VulkanContext::SelectPhysicalDevice()
{
}

SharedPtr<VkDevice> VulkanContext::CreateVkDevice()
{
}





bool VulkanContext::CheckValidationLayerSupport()
{
}

DynamicArray<Str> VulkanContext::GetRequiredExtensions()
{
}

bool VulkanContext::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
}

bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device)
{
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
    PL_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

#pragma region 应用信息
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "VulkanRenderer";
    appInfo.pEngineName        = "VulkanRenderer";
    appInfo.apiVersion         = VK_API_VERSION_1_2;
#pragma endregion

    std::vector<const char*> instanceExtensions { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

}
