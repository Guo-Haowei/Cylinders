#include "DisplayManager.h"
#include "../common.h"
#include "../inputManager/KeyboardManager.h"
#include "../inputManager/MouseManager.h"
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int WIDTH;
int HEIGHT;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

GLFWwindow* DisplayManager::window;

void DisplayManager::createDisplay() {
  // init glfw
  glfwInit();

  // set all required options
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cylinders", nullptr, nullptr);

  if (window == nullptr) {
    std::cout << "====================================\n";
    std::cout << "ERROR::GLFW: Failed to create window\n";
    std::cout << "====================================\n";
  }

  glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
  glfwMakeContextCurrent(window);

  // set callbacks
  glfwSetKeyCallback(window, keyCallback);
  glfwSetScrollCallback(window, scrollCallback);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "======================================\n";
    std::cout << "ERROR::GLEW: Failed to initialize GLEW\n";
    std::cout << "======================================\n";
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);

  // cull face
}

void DisplayManager::prepareDisplay() {
  glfwPollEvents();
}

void DisplayManager::updateDisplay() {
  glfwSwapBuffers(window);
}

void DisplayManager::cleanDisplay() {
  glfwTerminate();
}

bool DisplayManager::shouldCloseDisplay() {
  return glfwWindowShouldClose(window);
}

long double DisplayManager::getTime() {
  return glfwGetTime();
}

void DisplayManager::getCursorPos(double* x, double* y) {
  glfwGetCursorPos(window, x, y);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      KeyboardManager::setKeyDown(key);
    else if (action == GLFW_RELEASE)
      KeyboardManager::setKeyUp(key);
  }
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  MouseManager::xScrollOffset = xOffset;
  MouseManager::yScrollOffset = yOffset;
}
