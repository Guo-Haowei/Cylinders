#define _USE_MATH_DEFINES
#include "CylinderList.h"
#include "../common.h"
#include "../entities/Camera.h"
#include "../inputManager/MouseManager.h"
#include "../inputManager/KeyboardManager.h"
#include "../IO/IO.h"
#include "../renderEngine/DisplayManager.h"
#include "../maths/Maths.h"
#include <math.h>
#include <iostream>
using std::abs;
using std::cout;
using std::endl;

#include "../cyl/geometry.h"
#include "../cyl/cylinder.h"
#include "../cyl/unionfind.h"
#include "../cyl/cylIntersect.h"

Space W2 = SCreate("2-space", 2);
Frame F2 = StdFrame(W2);
Space W3 = SCreate("3-space", 3);
Frame F3 = StdFrame(W3);

Entity* CylinderList::selected = nullptr;
int CylinderList::selectedEntry = -1;

vector<Entity*> CylinderList::cylinders;

const float W = 400.0f, H = 300.0f, D = 500.0f;

// helpers
Cylinder createCylinderFromEntity(Entity* entity);
int intersect(Entity* cyl1, Entity* cyl2);

typedef struct Node {
  Entity* entity;
  Node* parent;
  int color;
} Node;

Node* find(Node* node) {
  Node* root = node;
  while (root != root->parent) {
    root = root->parent;
  }

  return root;
}

void unionNodes(Node* n1, Node* n2) {
  Node *r1 = find(n1);
  Node *r2 = find(n2);
  if (r1 != r2)
    r2->parent = r1;
}

void CylinderList::update(RawModel& model) {
  if (KeyboardManager::isKeyPressed(KEY_O) && cylinders.size() > 0) {
    IO::write(cylinders);
  }
  // create
  if (KeyboardManager::isKeyPressed(KEY_C) && cylinders.size() < 10) {
    int color = cylinders.size();
    Entity* entity = new Entity(model, glm::vec3(COLORS[color*3], COLORS[color*3+1], COLORS[color*3+2]), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1, 0.1, 1));
    cylinders.push_back(entity);
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
    }
  }

  if (KeyboardManager::isKeyPressed(KEY_TAB)) {
    MouseManager::setMode(SCENE);
    selected = nullptr;
  }

  // scale
  if (!MouseManager::buttonDown(LEFT_BUTTON) && !MouseManager::buttonDown(RIGHT_BUTTON) && selected) {
    // height
    float newHeight = selected->getScale().y - MouseManager::yScrollOffset * 0.1f;
    newHeight = newHeight < 0.1f ? 0.1f : newHeight;
    // radius
    float newRadius = selected->getScale().x - (KeyboardManager::isKeyDown(KEY_LSB) - KeyboardManager::isKeyDown(KEY_RSB)) * 0.1f;
    newRadius = newRadius < 0.1f ? 0.1f : newRadius;
    selected->setScale(glm::vec3(newRadius, newHeight, newRadius));
  }

  // transform
  else if (MouseManager::buttonDown(RIGHT_BUTTON) && selected) {
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
    if (std::isnan(a.x) || std::isnan(a.y) || std::isnan(a.z) || !selected)
      return;
    glm::mat4 rotationMatrix = Maths::calculateRotationMatrix(P0, P1, a);
    if (!std::isnan(rotationMatrix[0].x))
      selected->changeRotation(rotationMatrix);
  }

  // change color
  Node nodes[cylinders.size()];
  for (int i = 0; i < cylinders.size(); ++i) {
    nodes[i].parent = &nodes[i];
    nodes[i].entity = cylinders[i];
    nodes[i].color = i;
  }

  for (int i = 0; i < cylinders.size(); ++i) {
    for (int j = i + 1; j < cylinders.size(); ++j) {
      try {
        if (intersect(nodes[i].entity, nodes[j].entity)) {
          unionNodes(&nodes[i], &nodes[j]);
        }
      } catch (std::string &e) {
        if (e == "VVDot")
          IO::write(cylinders, "VVDot");
        exit(-1);
      }
    }
  }

  for (int i = 0; i < cylinders.size(); ++i) {
    int entry = find(&nodes[i])->color;
    cylinders[i]->setColor(glm::vec3(COLORS[entry*3], COLORS[entry*3+1], COLORS[entry*3+2]));
  }
}

glm::vec3 CylinderList::calculateCenterPoint() {
  glm::vec3 result = glm::vec3(0, 0, 0);
  if (cylinders.size() == 0)
    return result;
  for (int i = 0; i < CylinderList::cylinders.size(); ++i) {
    result += CylinderList::cylinders[i]->getPos();
  }
  return result / (float)CylinderList::cylinders.size();
}

void CylinderList::clean() {
  for (int i = 0; i < cylinders.size(); ++i)
    delete cylinders[i];

  cylinders.clear();
}

Cylinder createCylinderFromEntity(Entity* entity) {
  Cylinder c;
  c.r = entity->getScale().x;
  c.h = entity->getScale().y * 2.0f;
  glm::vec4 y4d = entity->getRotationMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
  glm::vec3 y = (float)c.h / 2.0f * glm::vec3(y4d.x, y4d.y, y4d.z);
  glm::vec3 A = entity->getPos() - y;
  glm::vec3 B = entity->getPos() + y;
  c.P = PCreate3(F3, A.x, A.y, A.z);
  c.A = c.P;
  c.B = PCreate3(F3, B.x, B.y, B.z);
  glm::vec3 v = glm::normalize(B - A);
  c.v = VCreate3(F3, v.x, v.y, v.z);

  return c;
}

int intersect(Entity* cyl1, Entity* cyl2) {
  return CylIntersect(createCylinderFromEntity(cyl1), createCylinderFromEntity(cyl2));
}
