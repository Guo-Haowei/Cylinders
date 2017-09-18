#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(RawModel& model, glm::vec3 color, glm::vec3 pos, glm::vec3 rot, float scale): model(model), color(color), pos(pos), rot(rot), scale(scale) { }

void Entity::changePosition(float dx, float dy, float dz) {
  pos.x += dx;
  pos.y += dy;
  pos.z += dz;
}

void Entity::changeRotation(float dx, float dy, float dz) {
  rot.x += (dx);
  rot.y += (dy);
  rot.z += (dz);
}

RawModel& Entity::getModel() const {
  return model;
}

glm::vec3 Entity::getPos() const {
  return pos;
}

glm::vec3 Entity::getRot() const {
  return rot;
}

float Entity::getScale() const {
  return scale;
}

glm::vec3 Entity::getColor() const {
  return color;
}

glm::mat4 Entity::createTransformationMatrix() {
  glm::mat4 matrix = glm::mat4(1);
  matrix = glm::translate(matrix, pos);
  matrix = glm::rotate(matrix, rot.x, glm::vec3(1, 0, 0));
  matrix = glm::rotate(matrix, rot.y, glm::vec3(0, 1, 0));
  matrix = glm::rotate(matrix, rot.z, glm::vec3(0, 0, 1));
  matrix = glm::scale(matrix, glm::vec3(scale));
  return matrix;
}
