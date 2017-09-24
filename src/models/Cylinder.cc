#define _USE_MATH_DEFINES
#include "Cylinder.h"
#include "../common.h"
#include "../entities/Camera.h"
#include "../inputManager/MouseManager.h"
#include "../inputManager/KeyboardManager.h"
#include "../renderEngine/DisplayManager.h"
#include <glm/glm.hpp>
#include <math.h>
#include <iostream>

Entity* Cylinder::selected = nullptr;
int Cylinder::selectedEntry = -1;

vector<Entity*> Cylinder::cylinders;

void Cylinder::update(RawModel& model) {
  // create
  if (KeyboardManager::isKeyPressed(KEY_C) && cylinders.size() < 10) {
    Entity* entity = new Entity(model, glm::vec3(0.6, 0.8, 0.8), glm::vec3(2 * cylinders.size()), glm::vec3(0), glm::vec3(1, 0.1, 1));
    cylinders.push_back(entity);
  }
  for (auto& it: cylinders) {
    // it->changeRotation(0.01, 0.01, 0.01);
    it->setColor(glm::vec3(0.3));
  }

  // delete all
  if (KeyboardManager::isKeyPressed(KEY_DEL)) {
    clean();
    selected = nullptr;
    selectedEntry = -1;
    MouseManager::setMode(SCENE);
  }

  // delete selected
  if (KeyboardManager::isKeyPressed(KEY_BACKSPACE)) {
    cylinders.erase(cylinders.begin() + selectedEntry);
    delete selected;
    selected = nullptr;
    selectedEntry = -1;
    MouseManager::setMode(SCENE);
  }

  // check if 0 - 9 is pressed
  for (int i = KEY_0; i <= KEY_9; ++i) {
    if (i - KEY_0 + 1 > cylinders.size())
      break;

    if (KeyboardManager::isKeyPressed(i)) {
      selected = cylinders[i - KEY_0];
      selectedEntry = i - KEY_0;

      MouseManager::setMode(OBJECT);
      DisplayManager::showCursor();
    }
  }

  if (KeyboardManager::isKeyPressed(KEY_TAB)) {
    DisplayManager::hideCursor();
    MouseManager::setMode(SCENE);
  }

  if (!selected || MouseManager::getMouseMode() != OBJECT)
    return;

  selected->setColor(glm::vec3(1.0f));

  // scale
  if (!MouseManager::buttonDown(LEFT_BUTTON) && !MouseManager::buttonDown(RIGHT_BUTTON)) {
    // height
    float newHeight = selected->getScale().y - MouseManager::yScrollOffset * 0.1f;
    newHeight = newHeight < 0.1f ? 0.1f : newHeight;
    // radius
    float newRadius = selected->getScale().x - (KeyboardManager::isKeyDown(KEY_LSB) - KeyboardManager::isKeyDown(KEY_RSB)) * 0.1f;
    newRadius = newRadius < 0.1f ? 0.1f : newRadius;
    selected->setScale(glm::vec3(newRadius, newHeight, newRadius));
  }

  // transform
  else if (MouseManager::buttonDown(LEFT_BUTTON)) {
    glm::vec3 camPos = Camera::getPos();
    glm::vec3 objPos = selected->getPos();
    float unitDistance =
      ((camPos.x - objPos.x) * (camPos.x - objPos.x) +
      (camPos.y - objPos.y) * (camPos.y - objPos.y) +
      (camPos.z - objPos.z) * (camPos.z - objPos.z));
    float deltaX = (MouseManager::currentX - MouseManager::lastX) * unitDistance / 4000.0f;
    float deltaY = (MouseManager::lastY - MouseManager::currentY) * unitDistance / 4800.0f;
    float deltaZ = MouseManager::yScrollOffset * unitDistance / 2000.0f;
    glm::vec3 movement = Camera::getUp() * deltaY + Camera::getRight() * deltaX + Camera::getFront() * deltaZ;
    selected->changePosition(movement.x, movement.y, movement.z);
  }
}

void Cylinder::clean() {
  for (auto& it: cylinders)
    delete it;

  cylinders.clear();
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
