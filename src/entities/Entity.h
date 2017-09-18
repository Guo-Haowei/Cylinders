#pragma once
#include "../models/RawModel.h"
#include <glm/glm.hpp>

class Entity {
private:
  RawModel& model;
  glm::vec3 pos;
  glm::vec3 rot;
  float scale;

  glm::vec3 color;
public:
  Entity(RawModel& model, glm::vec3 color, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 rot = glm::vec3(0, 0, 0), float scale = 1.0f);
  void changePosition(float dx, float dy, float dz);
  void changeRotation(float dx, float dy, float dz);

  RawModel& getModel() const;
  glm::vec3 getPos() const;
  glm::vec3 getRot() const;
  float getScale() const;
  glm::vec3 getColor() const;
  glm::mat4 createTransformationMatrix();
};
