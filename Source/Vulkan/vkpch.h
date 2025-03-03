#pragma once
#include <algorithm>
#include <any>
#include <cassert>
#include <chrono>
#include <concepts>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numbers>
#include <numeric>
#include <set>
#include <span>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb/stb_image.h>

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX
#pragma comment (lib, "vulkan-1.lib")
#endif
#include <vulkan/vulkan.h>

#define PL_ASSERT(x, ...) {if(!(x)) {std::cout << std::format("Assert Error : ", __VA_ARGS__, "\n"); __debugbreak();}}

#ifdef NDEBUG
    #define VL_CHECK(x) x
#else
    #define VL_CHECK(x) \
        do { \
            VkResult err = x; \
            if (err) { \
                std::cerr << std::format("Vulkan Error : {}\n", err); \
                __debugbreak(); \
            } \
        } while (0)
#endif
