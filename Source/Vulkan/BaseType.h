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


#define BIT(x) (1 << x)

/** ----------------------------类型--------------------------*/

using uint = unsigned int;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using size = size_t;

using Any = std::any;


/** ----------------------------容器--------------------------*/

//Set容器包装
template<typename T>
using Set = std::set<T>;

template<typename T>
using USet = std::unordered_set<T>;

// 动态数组容器包装
template<typename T>
using DynamicArray = std::vector<T>;

// Map容器包装
template<typename K, typename V>
using Map = std::map<K, V>;

template<typename K, typename V>
using UMap = std::unordered_map<K, V>;

// Array容器包装
template<typename T, size_t N>
using Array = std::array<T, N>;

// Stack容器包装
template<typename T>
using Stack = std::stack<T>;

// Deque容器包装
template<typename T>
using Deque = std::deque<T>;

// Pair容器包装
template<typename K, typename V>
using Pair = std::pair<K, V>;

template<typename K, typename V>
constexpr Pair<K, V> MakePair(K&& key, V&& value){
    return std::make_pair(std::forward<K>(key), std::forward<V>(value));}

template<typename T>
using Optional = std::optional<T>;

/** ---------------------------智能指针-------------------------*/

template<typename T >
using UniquePtr = std::unique_ptr<T>;

template<typename T, typename ... Args >
constexpr UniquePtr<T> MakeUnique(Args&& ... args){
    return std::make_unique<T>(std::forward<Args>(args)...);}

template<typename T >
using SharedPtr = std::shared_ptr<T>;

template<typename T, typename ... Args >
constexpr SharedPtr<T> MakeShared(Args&& ... args){
    return std::make_shared<T>(std::forward<Args>(args)...);}

template<typename T >
using WeakPtr = std::weak_ptr<T>;

template<typename T >
constexpr WeakPtr<T> MakeWeak(SharedPtr<T> ptr){
    return std::weak_ptr<T>(ptr);}


/** ---------------------------字符串-------------------------*/

//String容器包装
using String = std::string;
//StringView容器包装
using StringView = std::string_view;
//WString容器包装
using WString = std::wstring;
//WStringView容器包装
using WStringView = std::wstring_view;

using StringStream = std::stringstream;

//const char*容器包装
using Str = const char*;

/** ----------------------------函数--------------------------*/

template <typename T>
using Function = std::function<T>;

/** ----------------------------时间--------------------------*/

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Duration = std::chrono::duration<double>;

inline TimePoint Now(){
    return std::chrono::high_resolution_clock::now();}

inline Duration Elapsed(TimePoint start, TimePoint end){
    return end - start;}

/** ---------------------------工具函数-------------------------*/

template<typename T>
void HashCombine(size_t& seed, const T& v){
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);}
