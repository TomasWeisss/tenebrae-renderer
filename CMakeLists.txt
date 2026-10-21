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
file(GLOB_RECURSE CPP_SOURCE_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(${PROJECT_NAME}
  ${CPP_SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${PROJECT_NAME} PRIVATE VK_NO_PROTOTYPES)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(${PROJECT_NAME} PRIVATE
  Vulkan::Headers
  Vulkan::volk
  Vulkan::shaderc_combined
  glfw
  glm::glm
)


# Compiler flags
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:/W4 /utf-8 /Zc:preprocessor /Zc:__cplusplus>
  )
else ()
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Wno-sign-compare>
  )
endif ()


# Resources
# Debug reads res/ from the source tree (for hot reloading),
# every other config reads a copy placed next to the executable.
target_compile_definitions(${PROJECT_NAME} PRIVATE
  "RESOURCES_PATH=\"$<IF:$<CONFIG:Debug>,${CMAKE_CURRENT_SOURCE_DIR}/res/,res/>\""
)

add_custom_target(copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,true,copy_directory>
    "${CMAKE_CURRENT_SOURCE_DIR}/res" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)
add_dependencies(${PROJECT_NAME} copy_resources)


# Build and run debug
add_custom_target(run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  USES_TERMINAL
)
