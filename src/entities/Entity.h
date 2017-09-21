#pragma once
#include "../models/RawModel.h"
#include <glm/glm.hpp>

class Entity {
private:
  RawModel& model;
  glm::vec3 pos;
  glm::vec3 rot;
  glm::vec3 scale;

  glm::vec3 color;
public:
  Entity(RawModel& model, glm::vec3 color, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 rot = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
  void changePosition(float dx, float dy, float dz);
  void changeRotation(float dx, float dy, float dz);

  RawModel& getModel() const;
  glm::vec3 getPos() const;
  glm::vec3 getRot() const;
  glm::vec3 getScale() const;
  glm::vec3 getColor() const;
  void setPos(glm::vec3 pos);
  void setRot(glm::vec3 rot);
  void setScale(glm::vec3 scale);
  void setColor(glm::vec3 color);
  glm::mat4 createTransformationMatrix();
};
