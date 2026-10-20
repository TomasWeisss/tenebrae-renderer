include(FetchContent)

FetchContent_Declare(glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.5.1
    GIT_SHALLOW    TRUE
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
    OVERRIDE_FIND_PACKAGE
)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "Build glm as a static library") # header-only
