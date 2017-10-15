#include "Entity.h"
#include "../models/Loader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
using std::cout;
using std::vector;

int Entity::ID = 1;

Entity::Entity(RawModel& model, glm::vec3 color, glm::mat4 transformation, glm::vec3 scale): model(model), color(color), finalTransformation(transformation), scale(scale), id(Entity::ID++) {
  pos.x = transformation[3].x;
  pos.y = transformation[3].y;
  pos.z = transformation[3].z;
}

void Entity::changePosition(float dx, float dy, float dz) {
  // change transformation matrix
  glm::mat4 translation;
  translation = glm::translate(translation, glm::vec3(dx, dy, dz));
  finalTransformation = translation * finalTransformation;
  pos.x = finalTransformation[3].x;
  pos.y = finalTransformation[3].y;
  pos.z = finalTransformation[3].z;
}

int Entity::getID() const {
  return id;
}

RawModel& Entity::getModel() const {
  return model;
}

glm::vec3 Entity::getPos() const {
  return pos;
}

glm::vec3 Entity::getScale() const {
  return scale;
}

glm::vec3 Entity::getColor() const {
  return color;
}

glm::mat4 Entity::getTransformtationMatrix() const {
  return finalTransformation;
}

void Entity::changeRotation(glm::mat4 rot) {
  glm::mat4 T, TInverse;
  T = glm::translate(T, -pos);
  TInverse = glm::translate(TInverse, pos);
  finalTransformation = TInverse * rot * T * finalTransformation;
  // position also updated!
  pos.x = finalTransformation[3].x;
  pos.y = finalTransformation[3].y;
  pos.z = finalTransformation[3].z;
}

void Entity::changeRotation(glm::mat4 rot, glm::vec3 rotationCenter) {
  glm::mat4 T, TInverse;
  T = glm::translate(T, -rotationCenter);
  TInverse = glm::translate(TInverse, rotationCenter);
  finalTransformation = TInverse * rot * T * finalTransformation;
  // position also updated!
  pos.x = finalTransformation[3].x;
  pos.y = finalTransformation[3].y;
  pos.z = finalTransformation[3].z;
}

void Entity::setPos(glm::vec3 pos) {
  this->pos = pos;
}

void Entity::setScale(glm::vec3 scale) {
  this->scale = scale;
}

void Entity::setColor(glm::vec3 color) {
  this->color = color;
}

glm::mat4 Entity::createTransformationMatrix() {
  glm::mat4 scaleMatrix;
  scaleMatrix = glm::scale(scaleMatrix, scale);

  return finalTransformation * scaleMatrix;
}
