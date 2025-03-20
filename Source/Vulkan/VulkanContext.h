#pragma once
#include "Core/BaseType.h"
#include "Vulkan.h"

/**
 * @struct QueueFamilyIndices
 * @brief 队列族索引
 * @details
 * 储存与Vulkan物理设备相关的队列族索引 \n
 * 队列族索引是用于执行特定类型任务（图形渲染、计算、内存传输等）的队列集合
 */
struct QueueFamilyIndices
{
    Optional<uint> graphicsFamily;    ///< 图形队列组索引, 用于执行图形渲染任务
    Optional<uint> presentFamily;     ///< 呈现队列组索引，将渲染结果提交到窗口表面(Surface)
    Optional<uint> computeFamily;     ///< 计算队列组索引, 用于执行计算任务

    bool IsComplete () const {return graphicsFamily.has_value() && presentFamily.has_value();}
};

/**
 * @struct SwapChainSupportDetails
 * @brief 交换链支持
 * @details
 * 用于存储与交换链（Swap Chain）相关的支持信息
 */
struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;        ///< 表示表面(surface)的能力信息，包括交换链图像的最小/最大数量、图像的最小/最大尺寸等
    DynamicArray<VkSurfaceFormatKHR> formats;     ///< 表示表面支持的图像格式和颜色空间。VkSurfaceFormatKHR 包含 format（图像格式）和 colorSpace（颜色空间）两个字段。
    DynamicArray<VkPresentModeKHR> presentModes;  ///< 表示表面支持的呈现模式。VkPresentModeKHR 是一个枚举类型，定义了不同的呈现模式。
};


/**
 * @class VulkanContext
 * @brief Vulkan上下文
 * @details
 * 管理Vulkan实例、物理设备选择和逻辑设备创建 \n
 * 处理验证层和扩展
 */
class VulkanContext
{
public:
    /** 获取单例 */
    static VulkanContext& Get()
    {
        static VulkanContext instance;
        return instance;
    }

    /** 创建Vulkan实例 */
    void CreateInstance();

    /** 创建Vulkan表面 */
    void CreateSurface(GLFWwindow* window);

    /** 选择物理设备 */
    void SelectPhysicalDevice();

    /** 创建逻辑设备 */
    void CreateLogicalDevice();

    /** 寻找队列族索引*/
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;

    /** 查询交换链支持*/
    SwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice device) const;

    // Getter

    /** 获取Vulkan实例 */
    VkInstance GetInstance() { return s_VulkanInstance;}
    /** 获取物理设备 */
    VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice;}
    /** 获取逻辑设备 */
    VkDevice GetDevice() { return m_Device;}
    /** 获取表面*/
    VkSurfaceKHR GetSurface() { return m_Surface;}
    /** 获取图形队列*/
    VkQueue GetGraphicsQueue() const { return m_GraphicsQueue;}

    /** 获取呈现队列*/
    VkQueue GetPresentQueue() const { return m_PresentQueue;}

    void Init();

    VulkanContext();
    ~VulkanContext();
    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

private:
    /** 检查验证层支持*/
    bool CheckValidationLayerSupport();
    /** 获取所需扩展*/
    DynamicArray<Str> GetRequiredExtensions() const;
    /** 检查设备扩展支持*/
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    /** 检查设备是否适合*/
    bool IsDeviceSuitable(VkPhysicalDevice device);

private:
    VkPhysicalDevice m_PhysicalDevice;             ///< 物理设备
    VkDevice m_Device;                             ///< 逻辑设备
    VkSurfaceKHR m_Surface;                        ///< 表示Vulkan应用程序与窗口表面(surface)进行交互的句柄。
    static inline VkInstance s_VulkanInstance {VK_NULL_HANDLE};  ///< Vulkan实例句柄

    DynamicArray<Str> m_InstanceLayers;            ///< 实例层
    DynamicArray<Str> m_InstanceExtensions;        ///< 实例扩展

    const DynamicArray<Str> m_ValidationLayers {"VK_LAYER_KHRONOS_validation"};   ///< 验证层
    const DynamicArray<Str> m_DeviceExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME}; ///< 设备扩展

    VkQueue m_GraphicsQueue;                       ///< 图形队列
    VkQueue m_PresentQueue;                        ///< 呈现队列
    VkQueue m_ComputeQueue;                        ///< 计算队列

#ifdef NDEBUG
    const bool m_EnableValidationLayers = false;   ///< 不启用验证层,验证层用于检测和报告Vulkan应用程序中的错误和警告。
#else
    const bool m_EnableValidationLayers = true;    ///< 启用验证层,验证层用于检测和报告Vulkan应用程序中的错误和警告。
#endif
};
