cmake_minimum_required(VERSION 3.10)

add_library(glbasimac STATIC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE GLBASIMAC_SOURCES CONFIGURE_DEPENDS src/*)
if (MSVC)
target_compile_options(glbasimac PRIVATE /W3)
else()
target_compile_options(glbasimac PRIVATE -std=c++17 -Wall -Wextra)
endif()

target_sources(glbasimac PRIVATE ${GLBASIMAC_SOURCES})
target_include_directories(glbasimac PUBLIC ../glbasimac/)
include_directories(glbasimac)

