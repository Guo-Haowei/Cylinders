#pragma once
#include "../common.h"
#include <glm/glm.hpp>

const float SPEED = 0.1f;
const float SENSITIVITY = 0.01f;

class Camera {
private:
  static glm::vec3 position;
  static glm::vec3 center;
  static glm::vec3 front;
  static glm::vec3 up;
  static glm::vec3 right;
  static glm::vec3 worldUp;

  static float speed;
  static float sensitivity;
  static float mouseSensitivity;

  static void processScene();
public:
  static void update();
  static glm::vec3 getPos();
  static glm::vec3 getFront();
  static glm::vec3 getUp();
  static glm::vec3 getRight();
  static glm::mat4 getViewMatrix();
  static void setCenter(glm::vec3 point);
  static void updateCameraVectors();
};
