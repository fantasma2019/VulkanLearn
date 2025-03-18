#pragma once

namespace Utils
{

}
#define VK_CHECK(x) \
{ \
    VkResult result = x; \
    if (result != VK_SUCCESS) \
    { \
        PL_ASSERT(false, "Vulkan error: {}", result); \
    } \
}

//根据平台定义debugbreak
#ifdef PL_PLAT_WINDOWS
    #define PL_DEBUG_BREAK __debugbreak()
#elif PL_COMPILER_CLANG
    #define PL_DEBUG_BREAK __builtin_debugtrap()
#else
    #define PL_DEBUG_BREAK __builtin_debugtrap()
#endif

// DeBug模式下开启断言
#ifdef PL_DEBUG
    #define PL_ENABLE_ASSERT
#endif

#define PL_ENABLE_VERIFY


#ifdef PL_ENABLE_ASSERT
    #define PL_ASSERT(x, ...) {if(!(x)) {printf( __VA_ARGS__); PL_DEBUG_BREAK;}}
#else
    #define PL_ASSERT(x, ...)
#endif

#define PL_STATIC_ASSERT(x, ...) static_assert(x, __VA_ARGS__)
