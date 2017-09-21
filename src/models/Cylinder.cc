#define _USE_MATH_DEFINES
#include "Cylinder.h"
#include "../inputManager/KeyboardManager.h"
#include <glm/glm.hpp>
#include <math.h>
#include <iostream>

Entity* Cylinder::selected = nullptr;
vector<Entity*> Cylinder::cylinders;

void Cylinder::update(RawModel& model) {
  // check if c is pressed
  if (KeyboardManager::isKeyPressed(KEYS::KEY_C) && cylinders.size() < 10) {
    Entity* entity = new Entity(model, glm::vec3(0.3), glm::vec3(2 * cylinders.size()), glm::vec3(0), glm::vec3(1, 1 + cylinders.size(), 1));
    cylinders.push_back(entity);
  }
  for (auto& it: cylinders) {
    it->changeRotation(0.01, 0.01, 0.01);
    it->setColor(glm::vec3(0.3));
  }

  // check if 0 - 9 is pressed
  for (int i = KEYS::KEY_0; i <= KEYS::KEY_9; ++i) {
    if (i - KEYS::KEY_0 + 1 > cylinders.size())
      break;

    if (KeyboardManager::isKeyPressed(i))
      selected = cylinders[i - KEYS::KEY_0];
  }

  if (selected) {
    selected->setColor(glm::vec3(0.9));
    float newHeight = selected->getScale().y + KeyboardManager::isKeyPressed(KEYS::KEY_RIGHT_SQUARE_BRACKET) - KeyboardManager::isKeyPressed(KEYS::KEY_LEFT_SQUARE_BRACKET);
    selected->setScale(glm::vec3(1, newHeight < 1.0f ? 1.0f : newHeight, 1));
  }
}

void Cylinder::clean() {
  for (auto& it: cylinders)
    delete it;
}

RawModel Cylinder::createUniformCylinder(Loader& loader) {
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

  return loader.loadToVAO(vertices, normals);
}
