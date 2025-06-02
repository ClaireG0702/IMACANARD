cmake_minimum_required(VERSION 3.20)
project(IMACANARD)

# Sources
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${MY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/)

# Executable output path
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Librairie glm
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG efec5db081e3aad807d0731e172ac597f6a39447
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

# glbasimac
add_subdirectory(lib/glbasimac)
target_link_libraries(${PROJECT_NAME} PRIVATE glbasimac)

# GLFW (utilise ta version locale)
# add_subdirectory(lib/glbasimac/glfw)
target_include_directories(${PROJECT_NAME} BEFORE PRIVATE lib/glfw/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# Dear ImGui
file(GLOB IMGUI_SOURCES
    lib/imgui/*.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    lib/imgui
    lib/imgui/backends
    BEFORE PRIVATE lib/glfw/include  # S'assure que c'est la bonne version de GLFW
)

target_link_libraries(imgui PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# Autres includes pour IMGUI
target_include_directories(${PROJECT_NAME} PRIVATE lib/imgui lib/imgui/backends)

# Assets
include(CmakeUtils/files_and_folders.cmake)
Cool__target_copy_folder(${PROJECT_NAME} "assets")