#pragma once
#include "../models/RawModel.h"
#include <glm/glm.hpp>

class Entity {
private:
  static int ID;
  const int id;
  RawModel& model;
  glm::vec3 pos;
  glm::vec3 scale;
  glm::mat4 rotationMatrix;
  glm::mat4 finalTransformation;
  glm::vec3 color;
public:
  Entity(RawModel& model, glm::vec3 color, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), glm::mat4 rotationMatrix = glm::mat4(1.0f));
  void changePosition(float dx, float dy, float dz);

  int getID() const;
  RawModel& getModel() const;
  glm::vec3 getPos() const;
  glm::vec3 getScale() const;
  glm::vec3 getColor() const;
  glm::mat4 getRotationMatrix() const;
  void changeRotation(glm::mat4 rot);
  void changeRotation(glm::mat4 rot, glm::vec3 rotationCenter);
  void setPos(glm::vec3 pos);
  void setScale(glm::vec3 scale);
  void setColor(glm::vec3 color);
  void setRotation(glm::mat4 rot);
  glm::mat4 createTransformationMatrix();
};
