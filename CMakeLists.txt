cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)

project(exe)

# glfw
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

# opengl
find_package(OpenGL REQUIRED)

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE src/*.cc)
file(GLOB_RECURSE HEADER src/*.h)

add_definitions("-m64")
add_executable(exe ${HEADER} ${SOURCE})
target_link_libraries(exe ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} m)
