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
  glm::mat4 finalTransformation;
  glm::vec3 color;
public:
  Entity(RawModel& model, glm::vec3 color = glm::vec3(0.0f), glm::mat4 transformation = glm::mat4(1.0f), glm::vec3 scale = glm::vec3(1.0f));
  void changePosition(float dx, float dy, float dz);

  int getID() const;
  RawModel& getModel() const;
  glm::vec3 getPos() const;
  glm::vec3 getScale() const;
  glm::vec3 getColor() const;
  glm::mat4 getRotationMatrix() const;
  glm::mat4 getTransformtationMatrix() const;
  void changeRotation(glm::mat4 rot);
  void changeRotation(glm::mat4 rot, glm::vec3 rotationCenter);
  void setPos(glm::vec3 pos);
  void setTransform(glm::mat4 trans);
  void setScale(glm::vec3 scale);
  void setColor(glm::vec3 color);
  glm::mat4 createTransformationMatrix();
};
