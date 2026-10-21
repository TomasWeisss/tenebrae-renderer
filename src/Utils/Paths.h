#pragma once

/// Directory containing the running executable, independent of the working directory
std::filesystem::path GetExecutableDir();

/// Resolves a path relative to the resources folder, absolute paths are returned unchanged
std::filesystem::path GetResourcePath(const std::filesystem::path &path);
