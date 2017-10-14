#include "Entity.h"
#include "../models/Loader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using std::vector;

int Entity::ID = 1;

Entity::Entity(RawModel& model, glm::vec3 color, glm::vec3 pos, glm::vec3 scale, glm::mat4 rotationMatrix): model(model), color(color), pos(pos), scale(scale), rotationMatrix(rotationMatrix), id(Entity::ID++) {
  // initialize transformation matrix
  finalTransformation = glm::mat4(1.0f);
  // rotation
  glm::mat4 T, T_1, translation;
  T = glm::translate(T, -pos);
  T_1 = glm::translate(T_1, pos);
  glm::mat4 rotation = T_1 * rotationMatrix * T;
  // translation
  translation = glm::translate(translation, pos);
  finalTransformation = rotation * translation * finalTransformation;
}

void Entity::changePosition(float dx, float dy, float dz) {
  pos.x += dx;
  pos.y += dy;
  pos.z += dz;
  // change transformation matrix
  glm::mat4 translation;
  translation = glm::translate(translation, glm::vec3(dx, dy, dz));
  finalTransformation = translation * finalTransformation;
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

glm::mat4 Entity::getRotationMatrix() const {
  return rotationMatrix;
}

void Entity::changeRotation(glm::mat4 rot, glm::vec3 rotationCenter) {
  rotationMatrix = rot * rotationMatrix;
  // update transformation matrix as well
  glm::mat4 T, T_1;
  T = glm::translate(T, rotationCenter - pos);
  T_1 = glm::translate(T_1, pos - rotationCenter);
  finalTransformation = T_1 * rot * T * finalTransformation;
}

void Entity::setPos(glm::vec3 pos) {
  this->pos = pos;
}

void Entity::setScale(glm::vec3 scale) {
  this->scale = scale;
}

void Entity::setRotation(glm::mat4 rot) {
  this->rotationMatrix = rot;
}

void Entity::setColor(glm::vec3 color) {
  this->color = color;
}

glm::mat4 Entity::createTransformationMatrix() {
  glm::mat4 scaleMatrix;
  scaleMatrix = glm::scale(scaleMatrix, scale);

  return finalTransformation * scaleMatrix;
}
