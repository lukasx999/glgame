cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(GLGame)

include_directories(./libs/glad/include/)
add_executable(glgame main.cc)
target_link_libraries(glgame PRIVATE glfw)
