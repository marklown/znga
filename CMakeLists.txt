cmake_minimum_required(VERSION 2.8.9)
project(znga)

set (CMAKE_CXX_STANDARD 11)

file(GLOB SRC "source/*.cpp")

include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

add_executable(znga ${SRC})

target_link_libraries(znga ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES})