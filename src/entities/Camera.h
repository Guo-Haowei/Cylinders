#pragma once
#include "../common.h"
#include <glm/glm.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.01f;

class Camera {
private:
  static glm::vec3 position;
  static glm::vec3 front;
  static glm::vec3 up;
  static glm::vec3 right;
  static glm::vec3 worldUp;

  static float pitch;
  static float yaw;
  static float zoom;
  static float speed;
  static float sensitivity;
  static float mouseSensitivity;

  static void processScene();
public:
  static void update();
  static glm::mat4 getViewMatrix();
  static float getZoom();
  static void updateCameraVectors();
};
