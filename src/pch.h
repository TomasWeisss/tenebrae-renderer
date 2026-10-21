#pragma once

// Std
#include <algorithm>
#include <array>
#include <chrono>
#include <initializer_list>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <mutex>

// String
#include <cstring>
#include <format>
#include <sstream>
#include <string>

// Files
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <iostream>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

// Vulkan
#include <Volk/volk.h>

template <typename T>
using Ref = std::shared_ptr<T>;

struct free_deleter
{
    template <typename T>
    void operator()(T *p) const
    {
        std::free(const_cast<std::remove_const_t<T> *>(p));
    }
};
template <typename T>
using unique_ptr_free = std::unique_ptr<T, free_deleter>;
using void_ptr = unique_ptr_free<void>;

#include "Utils/Paths.h"
#define ABSOLUTE_RESOURCE_PATH(x) GetResourcePath(x)

// Logging
#include "Utils/Logger.h"
#define COLOR_WHITE "\033[0m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RED "\033[31m"

#define INTERNAL_LOG(color, format_str, ...) Logger::Log(std::filesystem::path(__FILE__).stem().string(), Logger::Format(format_str __VA_OPT__(,) __VA_ARGS__), color)
#define LOG(format_str, ...) INTERNAL_LOG(Logger::Severity::Info, format_str, ##__VA_ARGS__)
#define LOG_WARNING(format_str, ...) INTERNAL_LOG(Logger::Severity::Warning, format_str, ##__VA_ARGS__)
#define LOG_ERROR(format_str, ...) INTERNAL_LOG(Logger::Severity::Error, format_str, ##__VA_ARGS__)

#include "Utils/debugbreak.h"
