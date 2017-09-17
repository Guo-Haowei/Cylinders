#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
  position = glm::vec3(0.0f, 0.0f, 0.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  worldUp = up;
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  yaw = YAW;
  pitch = PITCH;
  speed = SPEED;
  sensitivity = SENSITIVITY;
  zoom = ZOOM;
  updateCameraVectors();
}

void Camera::update() {
  processKeyboard();
  processMouseMove();
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

float Camera::getZoom() const {
  return zoom;
}

void Camera::processKeyboard() {
}

void Camera::processMouseMove() {
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
