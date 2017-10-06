cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)

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

set(SOURCE
  src/cyl/geometry.cc
  src/cyl/cylIntersect.cc
  src/cyl/matrix.cc
  src/cyl/miller87.cc
  src/cyl/polyroots.cc
  src/cyl/s3d.cc
  src/entities/Camera.cc
  src/entities/Entity.cc
  src/inputManager/KeyboardManager.cc
  src/inputManager/MouseManager.cc
  src/IO/IO.cc
  src/models/CylinderList.cc
  src/models/Loader.cc
  src/models/RawModel.cc
  src/maths/Maths.cc
  src/renderEngine/DisplayManager.cc
  src/renderEngine/Renderer.cc
  src/shaders/EntityShader.cc
  src/shaders/ShaderProgram.cc
  src/main.cc
  )

set(HEADER
  src/common.h
  src/cyl/cylinder.h
  src/cyl/geometry.h
  src/cyl/cylIntersect.h
  src/cyl/matrix.h
  src/cyl/miller87.h
  src/cyl/polyroots.h
  src/cyl/s3d.h
  src/entities/Camera.h
  src/entities/Entity.h
  src/inputManager/KeyboardManager.h
  src/inputManager/MouseManager.h
  src/IO/IO.h
  src/models/CylinderList.h
  src/models/Loader.h
  src/models/RawModel.h
  src/maths/Maths.h
  src/renderEngine/DisplayManager.h
  src/renderEngine/Renderer.h
  src/shaders/EntityShader.h
  src/shaders/ShaderProgram.h
  )

add_definitions("-m64")
add_executable(exe ${HEADER} ${SOURCE})
target_link_libraries(exe ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} m)
