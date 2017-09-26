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
using std::abs;
using std::cout;
using std::endl;

Entity* Cylinder::selected = nullptr;
int Cylinder::selectedEntry = -1;

vector<Entity*> Cylinder::cylinders;

const float W = 400.0f, H = 300.0f, D = 500.0f;

void Cylinder::update(RawModel& model) {
  // create
  if (KeyboardManager::isKeyPressed(KEY_C) && cylinders.size() < 10) {
    Entity* entity = new Entity(model, glm::vec3(0.3), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1, 0.1, 1));
    cylinders.push_back(entity);
  }
  for (auto& it: cylinders) {
    // it->changeRotation(0.01, 0.01, 0.01);
    it->setColor(glm::vec3(0.3));
  }

  // delete all
  if (KeyboardManager::isKeyPressed(KEY_DEL) && selected) {
    clean();
    selected = nullptr;
    selectedEntry = -1;
    MouseManager::setMode(SCENE);
  }

  // delete selected
  if (KeyboardManager::isKeyPressed(KEY_BACKSPACE) && selected) {
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
  else if (MouseManager::buttonDown(RIGHT_BUTTON)) {
    glm::vec3 camPos = Camera::getPos();
    glm::vec3 objPos = selected->getPos();
    float unitDistance =
      ((camPos.x - objPos.x) * (camPos.x - objPos.x) +
      (camPos.y - objPos.y) * (camPos.y - objPos.y) +
      (camPos.z - objPos.z) * (camPos.z - objPos.z));
    float deltaX = (MouseManager::currentX - MouseManager::lastX) * unitDistance / 4000.0f;
    float deltaY = (MouseManager::lastY - MouseManager::currentY) * unitDistance / 4800.0f;
    float deltaZ = MouseManager::yScrollOffset * unitDistance / 800.0f;
    glm::vec3 movement = Camera::getUp() * deltaY + Camera::getRight() * deltaX + Camera::getFront() * deltaZ;
    selected->changePosition(movement.x, movement.y, movement.z);
  }

  // rotation
  else if (MouseManager::buttonDown(LEFT_BUTTON)) {
    // construct two points
    float x0 = MouseManager::lastX - W / 2;
    float y0 = MouseManager::lastY - H / 2;
    glm::vec3 P0 = glm::vec3(x0, y0, sqrt(D*D - y0 * y0));
    float x1 = MouseManager::currentX - W / 2;
    float y1 = MouseManager::currentY - H / 2;
    glm::vec3 P1 = glm::vec3(x1, y1, sqrt(D*D - y1 * y1));
    // rotation vector
    glm::vec3 a = glm::normalize(glm::cross(P0, P1));
    if (isnan(a.x) || isnan(a.y) || isnan(a.z))
      return;
    // rotation degree
    float cosTheta = glm::dot(P0, P1) / (glm::length(P0) * glm::length(P1));
    float sinTheta = sqrt(1 - cosTheta * cosTheta);

    float xAbs = abs(a.x), yAbs = abs(a.y), zAbs = abs(a.z);
    if (1) {
    // if (a.x >= a.y && a.x >= a.z) {
      // project onto xz plane
      float XZ = sqrt(a.x * a.x + a.z * a.z);
      float cosPhi = a.x / XZ;
      float sinPhi = a.z / XZ;

      glm::mat4 Ry_phi(1.0f);
      Ry_phi[0].x = cosPhi; Ry_phi[0].z = sinPhi;
      Ry_phi[2].x = -sinPhi; Ry_phi[2].z = cosPhi;

      glm::mat4 Ryphi(1.0f);
      Ryphi[0].x = cosPhi; Ryphi[0].z = -sinPhi;
      Ryphi[2].x = sinPhi; Ryphi[2].z = cosPhi;

      float cosPsi = XZ;
      float sinPsi = a.y;

      glm::mat4 Rz_psi(1.0f);
      Rz_psi[0].x = cosPsi; Rz_psi[0].y = sinPsi;
      Rz_psi[1].x = -sinPsi; Rz_psi[1].y = cosPsi;

      glm::mat4 Rzpsi(1.0f);
      Rzpsi[0].x = cosPsi; Rzpsi[0].y = -sinPsi;
      Rzpsi[1].x = sinPsi; Rzpsi[1].y = cosPsi;

      glm::mat4 RxTheta(1.0f);
      RxTheta[1].y = cosTheta; RxTheta[1].z = -sinTheta;
      RxTheta[2].y = sinTheta; RxTheta[2].z = cosTheta;

      selected->changeRotation(Ryphi * Rzpsi * RxTheta * Rz_psi * Ry_phi);
    }
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
