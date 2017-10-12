#include "Camera.h"
#include "../inputManager/KeyboardManager.h"
#include "../inputManager/MouseManager.h"
#include "../models/CylinderList.h"
#include "../maths/Maths.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using std::cout;

glm::vec3 Camera::position = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::right;
glm::vec3 Camera::worldUp = up;
glm::vec3 Camera::front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::mat4 Camera::rotationMatrix = glm::mat4(1.0f);
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

  if (MouseManager::buttonDown(LEFT_BUTTON) && !CylinderList::selected) {
    // construct two points
    float x0 = -MouseManager::lastX + 400;
    float y0 = -MouseManager::lastY + 300;
    glm::vec3 P0 = glm::vec3(x0, y0, sqrt(250000 - y0 * y0));
    float x1 = -MouseManager::currentX + 400;
    float y1 = -MouseManager::currentY + 300;
    glm::vec3 P1 = glm::vec3(x1, y1, sqrt(250000 - y1 * y1));
    // rotation vector
    glm::vec3 a = glm::normalize(glm::cross(P0, P1));
    if (!std::isnan(a.x) && !std::isnan(a.y) && !std::isnan(a.z)) {
      glm::mat4 rotationMatrix = Maths::calculateRotationMatrix(P0, P1, a);
      if (!Maths::isNaNMatrix(rotationMatrix)) {
        glm::mat4 T, T_1;
        glm::vec3 center = CylinderList::calculateCenterPoint();
        T = glm::translate(T, -center);
        T_1 = glm::translate(T_1, center);
        glm::vec4 pos = T_1 * rotationMatrix * T * glm::vec4(position.x, position.y, position.z, 1.0f);
        position.x = pos.x;
        position.y = pos.y;
        position.z = pos.z;
      }
    }
  }

  int sign = MouseManager::yScrollOffset < 0 ? 1 : (MouseManager::yScrollOffset > 0 ? -1 : 0);
  glm::vec3 detlaZ = speed * sign * front;
  if (glm::length(position + detlaZ) > 1)
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
