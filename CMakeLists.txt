cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)

project(exe)

# conditional flag
option(WITH_FPS "display fps" OFF)
option(WITH_AA "anti aliasing" ON)

if(WITH_FPS)
  add_definitions(-DCOUNT_FPS)
endif()

if(WITH_AA)
  add_definitions(-DANTI_ALIASING)
endif()

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

file(GLOB_RECURSE SRCS "*.cc")
file(GLOB_RECURSE CSRCS "cyl/*.c")
file(GLOB_RECURSE HDRS "*.h")

add_definitions("-m64")
add_executable(exe ${CSRCS} ${SRCS} ${HDRS})
target_link_libraries(exe ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} m)
