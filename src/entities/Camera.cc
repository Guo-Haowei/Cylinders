#include "Camera.h"
#include "../inputManager/KeyboardManager.h"
#include "../inputManager/MouseManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera() {
  position = glm::vec3(0.0f, 0.0f, 0.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  worldUp = up;
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  yaw = YAW;
  pitch = PITCH;
  speed = SPEED;
  sensitivity = SENSITIVITY;
  mouseSensitivity = 0.1f;
  zoom = ZOOM;
  updateCameraVectors();
}

void Camera::update() {
  processScene();
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

float Camera::getZoom() const {
  return zoom;
}

void Camera::processScene() {
  if (MouseManager::getMouseMode() != SCENE)
    return;

  // camera position
  if (KeyboardManager::isKeyDown(KEYS::KEY_W))
      position += speed * up;
  if (KeyboardManager::isKeyDown(KEYS::KEY_S))
      position -= speed * up;
  if (KeyboardManager::isKeyDown(KEYS::KEY_A))
      position -= speed * right;
  if (KeyboardManager::isKeyDown(KEYS::KEY_D))
      position += speed * right;

  int sign = MouseManager::yScrollOffset < 0 ? -1 : (MouseManager::yScrollOffset > 0 ? 1 : 0);
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
