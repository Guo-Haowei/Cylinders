#include "Entity.h"
#include "../models/Loader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using std::vector;

Entity::Entity(RawModel& model, glm::vec3 color, glm::vec3 pos, glm::vec3 scale, glm::mat4 rotationMatrix, glm::mat4 rotationCenterMatrix): model(model), color(color), pos(pos), scale(scale), rotationMatrix(rotationMatrix), rotationCenter(glm::vec3(0, 0, 0)), rotationCenterMatrix(rotationCenterMatrix) { }

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

glm::mat4 Entity::getRotationMatrix() const {
  return rotationMatrix;
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

void Entity::setRotationCenterMatrix(glm::mat4 mat) {

}

void Entity::setRotationCenter(glm::vec3 center) {
  this->rotationCenter = center;
}

glm::mat4 Entity::createTransformationMatrix() {
  glm::mat4 matrix = glm::mat4(1);
  // rotate around cylinder center
  glm::mat4 T, T_1;
  T = glm::translate(T, -pos);
  T_1 = glm::translate(T_1, pos);
  matrix = T_1 * rotationMatrix * T;
  // rotate around scene center
  T = glm::translate(T, rotationCenter - pos);
  T_1 = glm::translate(T_1, rotationCenter + pos);
  matrix = T_1 * rotationCenterMatrix * T * matrix;
  // translate
  matrix = glm::translate(matrix, pos);
  // scale
  matrix = glm::scale(matrix, scale);
  return matrix;
}

RawModel Entity::createUniformCylinder() {
  vector<float> vertices;
  vector<float> normals;

  for (int a = 2; a + 2 < 360; a += 2) {
    float x1 = sin(a * M_PI / 180);
    float z1 = cos(a * M_PI / 180);
    float x2 = sin((a + 2) * M_PI / 180);
    float z2 = cos((a + 2) * M_PI / 180);
    // top surface
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(x1);
    vertices.push_back(1.0f);
    vertices.push_back(z1);
    vertices.push_back(x2);
    vertices.push_back(1.0f);
    vertices.push_back(z2);
    // top normals
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    normals.push_back(0.0f);

    // bottom surface
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(x1);
    vertices.push_back(-1.0f);
    vertices.push_back(z1);
    vertices.push_back(x2);
    vertices.push_back(-1.0f);
    vertices.push_back(z2);
    // top normals
    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(-1.0f);
    normals.push_back(0.0f);
  }

  for (int a = 0; a + 2 <= 360; a += 2) {
    float x1 = sin(a * M_PI / 180);
    float z1 = cos(a * M_PI / 180);
    float x2 = sin(((a + 2) % 360) * M_PI / 180);
    float z2 = cos(((a + 2) % 360) * M_PI / 180);
    // triangle 1
    vertices.push_back(x1);
    vertices.push_back(1.0f);
    vertices.push_back(z1);
    vertices.push_back(x2);
    vertices.push_back(1.0f);
    vertices.push_back(z2);
    vertices.push_back(x1);
    vertices.push_back(-1.0f);
    vertices.push_back(z1);
    // normal
    normals.push_back(x1 + x2);
    normals.push_back(0);
    normals.push_back(z1 + z2);
    normals.push_back(x1 + x2);
    normals.push_back(0);
    normals.push_back(z1 + z2);
    normals.push_back(x1 + x2);
    normals.push_back(0);
    normals.push_back(z1 + z2);
    // triangle 2
    vertices.push_back(x2);
    vertices.push_back(1.0f);
    vertices.push_back(z2);
    vertices.push_back(x2);
    vertices.push_back(-1.0f);
    vertices.push_back(z2);
    vertices.push_back(x1);
    vertices.push_back(-1.0f);
    vertices.push_back(z1);
    // normal
    normals.push_back(x1 + x2);
    normals.push_back(0);
    normals.push_back(z1 + z2);
    normals.push_back(x1 + x2);
    normals.push_back(0);
    normals.push_back(z1 + z2);
    normals.push_back(x1 + x2);
    normals.push_back(0);
    normals.push_back(z1 + z2);
  }

  return Loader::loadToVAO(vertices, normals);
}
