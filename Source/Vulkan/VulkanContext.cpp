#include "vkpch.h"
#include "VulkanContext.h"

SharedPtr<VkDevice> VulkanContext::CreateVkDevice()
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

VulkanContext::VulkanContext()
{
    Init();
}
