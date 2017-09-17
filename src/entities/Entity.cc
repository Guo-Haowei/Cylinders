#include "Entity.h"

Entity::Entity(RawModel& model, glm::vec3 pos, glm::vec3 rot, float scale): model(model), pos(pos), rot(rot), scale(scale) { }

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
