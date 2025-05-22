cmake_minimum_required(VERSION 3.20)
project(IMACANARD)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${MY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/)

# Set the folder where the executable will be created
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Librairies

# ---Add glbasimac---
include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG efec5db081e3aad807d0731e172ac597f6a39447
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

add_subdirectory(lib/glbasimac)
target_link_libraries(${PROJECT_NAME} PRIVATE glbasimac)

include(CmakeUtils/files_and_folders.cmake)
Cool__target_copy_folder(${PROJECT_NAME} "assets")