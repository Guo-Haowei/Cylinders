#pragma once
#include <glm/glm.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float pitch;
  float yaw;
public:
};
