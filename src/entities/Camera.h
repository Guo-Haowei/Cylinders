#pragma once
#include "../common.h"
#include <glm/glm.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.01f;

class Camera {
private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float pitch;
  float yaw;
  float zoom;
  float speed;
  float sensitivity;
  float mouseSensitivity;

  void updateCameraVectors();
  void processKeyboard();
  void processMouseMove();
public:
  Camera();

  void update();
  glm::mat4 getViewMatrix();
  float getZoom() const;
};
