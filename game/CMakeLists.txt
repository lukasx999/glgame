cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(Game)

include_directories(../libgfx/subprojects/glad/include)
include_directories(../libgfx/include)
link_directories(../libgfx/build)

add_executable(game main.cc)
target_link_libraries(game PRIVATE gfx glfw)
