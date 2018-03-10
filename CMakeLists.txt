cmake_minimum_required(VERSION 2.8.9)
project(znga)

set (CMAKE_CXX_STANDARD 11)

file(GLOB SRC "source/*.c")
file(GLOB INC "include/*.h")

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ASSIMP REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(include)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})

link_directories("/usr/local/lib")

add_executable(znga ${SRC} ${INC})

target_link_libraries(znga ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${ASSIMP_LIBRARIES})
