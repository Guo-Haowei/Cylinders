#define _USE_MATH_DEFINES
#include "Controls.h"
#include "MouseManager.h"
#include "KeyboardManager.h"
#include "../common.h"
#include "../models/CylinderList.h"
#include "../entities/Camera.h"
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

void Controls::update(RawModel& model) {
  if (KeyboardManager::isKeyPressed(KEY_M)) {
    IO::manual();
  }
  if (KeyboardManager::isKeyPressed(KEY_O) && CylinderList::cylinders.size() > 0) {
    IO::write(CylinderList::cylinders);
  }
  // create
  if (KeyboardManager::isKeyPressed(KEY_C) && CylinderList::cylinders.size() < 10) {
    int color = CylinderList::cylinders.size();
    Entity* entity = new Entity(model, glm::vec3(COLORS[color*3], COLORS[color*3+1], COLORS[color*3+2]), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1, 0.1, 1));
    CylinderList::cylinders.push_back(entity);
  }

  // delete all
  if (KeyboardManager::isKeyPressed(KEY_DEL) && CylinderList::selected) {
    CylinderList::clean();
    CylinderList::selected = nullptr;
    MouseManager::setMode(SCENE);
  }

  // delete selected
  if (KeyboardManager::isKeyPressed(KEY_BACKSPACE) && CylinderList::selected) {
    int i = 0;
    for (; i < CylinderList::cylinders.size(); ++i) {
      if (CylinderList::cylinders[i] == CylinderList::selected)
        break;
    }
    CylinderList::cylinders.erase(CylinderList::cylinders.begin() + i);
    delete CylinderList::selected;
    CylinderList::selected = nullptr;
    MouseManager::setMode(SCENE);
  }

  // check if 0 - 9 is pressed
  for (int i = KEY_0; i <= KEY_9; ++i) {
    if (i - KEY_0 + 1 > CylinderList::cylinders.size())
      break;

    if (KeyboardManager::isKeyPressed(i)) {
      CylinderList::selected = CylinderList::cylinders[i - KEY_0];

      MouseManager::setMode(OBJECT);
    }
  }

  if (KeyboardManager::isKeyPressed(KEY_TAB)) {
    MouseManager::setMode(SCENE);
    CylinderList::selected = nullptr;
  }

  // scale
  if (!MouseManager::buttonDown(LEFT_BUTTON) && !MouseManager::buttonDown(RIGHT_BUTTON) && CylinderList::selected) {
    // height
    float newHeight = CylinderList::selected->getScale().y - MouseManager::yScrollOffset * 0.1f;
    newHeight = newHeight < 0.1f ? 0.1f : newHeight;
    // radius
    float newRadius = CylinderList::selected->getScale().x - (KeyboardManager::isKeyDown(KEY_LSB) - KeyboardManager::isKeyDown(KEY_RSB)) * 0.1f;
    newRadius = newRadius < 0.1f ? 0.1f : newRadius;
    CylinderList::selected->setScale(glm::vec3(newRadius, newHeight, newRadius));
  }

  // transform
  else if (MouseManager::buttonDown(RIGHT_BUTTON) && !MouseManager::buttonDown(LEFT_BUTTON) && CylinderList::selected) {
    glm::vec3 dis = Camera::getPos() - CylinderList::selected->getPos();
    float unitDistance = dis.x * dis.x + dis.y * dis.y + dis.z * dis.z;
    float deltaX = (MouseManager::currentX - MouseManager::lastX) * unitDistance / 4000.0f;
    float deltaY = (MouseManager::lastY - MouseManager::currentY) * unitDistance / 4800.0f;
    float deltaZ = MouseManager::yScrollOffset * unitDistance / 800.0f;
    glm::vec3 movement = Camera::getUp() * deltaY + Camera::getRight() * deltaX + Camera::getFront() * deltaZ;
    CylinderList::selected->changePosition(movement.x, movement.y, movement.z);
  }

  // rotation
  else if (MouseManager::buttonDown(LEFT_BUTTON) && !MouseManager::buttonDown(RIGHT_BUTTON) && CylinderList::selected) {
    // construct two points
    float x0 = MouseManager::lastX - W / 2;
    float y0 = MouseManager::lastY - H / 2;
    glm::vec3 P0 = glm::vec3(x0, y0, sqrt(D*D - y0 * y0));
    float x1 = MouseManager::currentX - W / 2;
    float y1 = MouseManager::currentY - H / 2;
    glm::vec3 P1 = glm::vec3(x1, y1, sqrt(D*D - y1 * y1));
    // rotation vector
    glm::vec3 a = glm::normalize(glm::cross(P0, P1));
    if (std::isnan(a.x) || std::isnan(a.y) || std::isnan(a.z) || !CylinderList::selected)
      return;
    glm::mat4 rotationMatrix = Maths::calculateRotationMatrix(P0, P1, a);
    if (!Maths::isNaNMatrix(rotationMatrix))
      CylinderList::selected->changeRotation(rotationMatrix);
  }

  // change color
  Node nodes[CylinderList::cylinders.size()];
  for (int i = 0; i < CylinderList::cylinders.size(); ++i) {
    nodes[i].parent = &nodes[i];
    nodes[i].entity = CylinderList::cylinders[i];
    nodes[i].color = i;
  }

  for (int i = 0; i < CylinderList::cylinders.size(); ++i) {
    for (int j = i + 1; j < CylinderList::cylinders.size(); ++j) {
      try {
        if (intersect(nodes[i].entity, nodes[j].entity)) {
          unionNodes(&nodes[i], &nodes[j]);
        }
      } catch (std::exception& e) {
        cout << e.what() << endl;
        IO::write(CylinderList::cylinders, "VVDot");
        exit(-1);
      }
    }
  }

  for (int i = 0; i < CylinderList::cylinders.size(); ++i) {
    int entry = find(&nodes[i])->color;
    CylinderList::cylinders[i]->setColor(glm::vec3(COLORS[entry*3], COLORS[entry*3+1], COLORS[entry*3+2]));
  }
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
