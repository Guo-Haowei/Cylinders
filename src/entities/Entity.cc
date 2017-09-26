#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(RawModel& model, glm::vec3 color, glm::vec3 pos, glm::vec3 scale): model(model), color(color), pos(pos), scale(scale) {
  rotationMatrix = glm::mat4(1);
}

void Entity::changePosition(float dx, float dy, float dz) {
  pos.x += dx;
  pos.y += dy;
  pos.z += dz;
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

void Entity::changeRotation(glm::mat4 rot) {
  rotationMatrix = rot * rotationMatrix;
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
  glm::mat4 matrix = glm::mat4(1);
  // rotate
  matrix = rotationMatrix * matrix;
  // translate
  matrix = glm::translate(matrix, pos);
  // scale
  matrix = glm::scale(matrix, scale);
  return matrix;
}
