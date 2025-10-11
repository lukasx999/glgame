cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(Gfx)

add_library(gfx)
target_compile_options(gfx PRIVATE "-Wno-c23-extensions" "-Wall" "-Wextra" "-O3")
add_subdirectory(src)
include_directories(subprojects/glad/include)
include_directories(subprojects/stb)
include_directories(include)
target_link_libraries(gfx PRIVATE glfw)

add_executable(tests test.cc)
