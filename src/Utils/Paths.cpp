#include "Paths.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

std::filesystem::path GetExecutableDir()
{
#ifdef _WIN32
    std::wstring buffer(MAX_PATH, L'\0');
    DWORD length;
    // A full buffer means the path was truncated, so grow it and retry
    while ((length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()))) == buffer.size())
        buffer.resize(buffer.size() * 2);
    buffer.resize(length);
    return std::filesystem::path(buffer).parent_path();
#else
    return std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
}

std::filesystem::path GetResourcePath(const std::filesystem::path &path)
{
    // RESOURCES_PATH is absolute in Debug, in which case operator/ discards the executable dir
    static const std::filesystem::path resourcesDir = GetExecutableDir() / RESOURCES_PATH;
    return path.is_relative() ? (resourcesDir / path).lexically_normal() : path;
}
