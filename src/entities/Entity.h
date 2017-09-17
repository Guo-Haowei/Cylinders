#pragma once
#include "../models/RawModel.h"
#include <glm/glm.hpp>

class Entity {
private:
  RawModel& model;
  glm::vec3 pos;
  glm::vec3 rot;
  float scale;
public:
  Entity(RawModel& model, glm::vec3 pos, glm::vec3 rot, float scale);
  void changePosition(float dx, float dy, float dz);
  void changeRotation(float dx, float dy, float dz);

  RawModel& getModel() const;
  glm::vec3 getPos() const;
  glm::vec3 getRot() const;
  float getScale() const;
};
