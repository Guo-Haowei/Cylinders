#include "Camera.h"
#include "../inputManager/KeyboardManager.h"
#include "../inputManager/MouseManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

glm::vec3 Camera::position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::right;
glm::vec3 Camera::worldUp = up;
glm::vec3 Camera::front = glm::vec3(0.0f, 0.0f, -1.0f);
float Camera::yaw = YAW;
float Camera::pitch = PITCH;
float Camera::speed = SPEED;
float Camera::sensitivity = SENSITIVITY;
float Camera::mouseSensitivity = 0.1f;
float Camera::zoom = ZOOM;

void Camera::update() {
  processScene();
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

float Camera::getZoom() {
  return zoom;
}

void Camera::processScene() {
  if (MouseManager::getMouseMode() != SCENE)
    return;

  // camera position
  if (KeyboardManager::isKeyDown(KEY_W))
      position += speed * up;
  if (KeyboardManager::isKeyDown(KEY_S))
      position -= speed * up;
  if (KeyboardManager::isKeyDown(KEY_A))
      position -= speed * right;
  if (KeyboardManager::isKeyDown(KEY_D))
      position += speed * right;

  int sign = MouseManager::yScrollOffset < 0 ? 1 : (MouseManager::yScrollOffset > 0 ? -1 : 0);
  position += speed * sign * front;

  // camera rotation
  float xOffset = (MouseManager::currentX - MouseManager::lastX) * mouseSensitivity;
  float yOffset = (MouseManager::lastY - MouseManager::currentY) * mouseSensitivity;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  // calculate updated front vector
  glm::vec3 updatedFront;
  updatedFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  updatedFront.y = sin(glm::radians(pitch));
  updatedFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(updatedFront);
  // update right and up vector
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}
