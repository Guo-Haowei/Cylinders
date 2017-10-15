#include "Camera.h"
#include "../inputManager/KeyboardManager.h"
#include "../inputManager/MouseManager.h"
#include "../models/CylinderList.h"
#include "../maths/Maths.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using std::cout;

glm::vec3 Camera::position = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 Camera::center = glm::vec3 (0, 0, -1);
glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::right;
glm::vec3 Camera::worldUp = up;
glm::vec3 Camera::front = glm::vec3(0.0f, 0.0f, -1.0f);
float Camera::speed = SPEED;
float Camera::sensitivity = SENSITIVITY;
float Camera::mouseSensitivity = 0.04f;

void Camera::update() {
  processScene();
}

glm::vec3 Camera::getPos() {
  return position;
}

glm::vec3 Camera::getFront() {
  return front;
}

glm::vec3 Camera::getUp() {
  return up;
}

glm::vec3 Camera::getRight() {
  return right;
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

void Camera::setCenter(glm::vec3 point) {
  Camera::center = center;
}

void Camera::processScene() {
  if (MouseManager::getMouseMode() != SCENE)
    return;

  if (KeyboardManager::isKeyDown(KEY_S))
    position -= speed * front;
  if (KeyboardManager::isKeyDown(KEY_W))
    position += speed * front;
  if (KeyboardManager::isKeyDown(KEY_A))
    position -= speed * right;
  if (KeyboardManager::isKeyDown(KEY_D))
    position += speed * right;

  int sign = MouseManager::yScrollOffset < 0 ? 1 : (MouseManager::yScrollOffset > 0 ? -1 : 0);
  glm::vec3 detlaZ = speed * sign * front;
  position += speed * sign * front;

  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  // always look at center
  front = glm::normalize(CylinderList::calculateCenterPoint() - position);
  // update right and up vector
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}
