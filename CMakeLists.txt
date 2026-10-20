cmake_minimum_required(VERSION 3.25...4.4)
project(tenebrae
  VERSION 1.0
  LANGUAGES C CXX
)

# Dependencies
find_package(Vulkan REQUIRED COMPONENTS volk shaderc_combined)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Target
file(GLOB CPP_SOURCE_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(tenebrae
  ${CPP_SOURCE_FILES}
)

target_compile_features(tenebrae PRIVATE cxx_std_20)
set_target_properties(tenebrae PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(tenebrae PRIVATE
  Vulkan::Vulkan
  Vulkan::volk
  Vulkan::shaderc_combined
  glfw
  glm::glm
)

# Build and run debug
add_custom_target(run
  COMMAND tenebrae
  DEPENDS tenebrae
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  USES_TERMINAL
)
