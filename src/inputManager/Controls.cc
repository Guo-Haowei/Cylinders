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
    Entity* entity = new Entity(model, glm::vec3(COLORS[color*3], COLORS[color*3+1], COLORS[color*3+2]), glm::mat4(1.0f), glm::vec3(1.0f, 0.1f, 1.0f));
    CylinderList::cylinders.push_back(entity);
    CylinderList::selected = entity;
    MouseManager::setMode(OBJECT);
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

  else if (MouseManager::buttonDown(LEFT_BUTTON) && !MouseManager::buttonDown(RIGHT_BUTTON)) {
    float x0 = MouseManager::lastX - W;
    float y0 = H - MouseManager::lastY;
    float x1 = MouseManager::currentX - W;
    float y1 = H - MouseManager::currentY;
    glm::mat4 rotationMatrix;
    if (x0 * x0 + y0 * y0 <= R * R && x1 * x1 + y1 * y1 <= R * R) {
      glm::vec3 P0 = glm::vec3(x0, y0, -sqrt(R * R - y0 * y0 - x0 * x0));
      glm::vec3 P1 = glm::vec3(x1, y1, -sqrt(R * R - y1 * y1 - x1 * x1));
      glm::vec3 a = glm::normalize(glm::cross(P0, P1));
      if (Maths::isNaNVector(a))
        return;
      float cosTheta = glm::dot(P0, P1) / (glm::length(P0) * glm::length(P1));
      float sinTheta = -sqrt(1 - cosTheta * cosTheta);
      rotationMatrix = Maths::calculateRotationMatrix(cosTheta, sinTheta, a);
    } else {
      float cosA = x0 / sqrt(x0 * x0 + y0 * y0), sinA = y0 / sqrt(x0 * x0 + y0 * y0);
      float cosB = x1 / sqrt(x1 * x1 + y1 * y1), sinB = y1 / sqrt(x1 * x1 + y1 * y1);
      float cosTheta = cosA * cosB + sinA * sinB;
      float sinTheta = sinA * cosB - cosA * sinB;
      rotationMatrix = Maths::calculateRotationMatrix(cosTheta, sinTheta, Camera::getFront());
    }

    if (Maths::isNaNMatrix(rotationMatrix))
      return;
    if (CylinderList::selected) {
      CylinderList::selected->changeRotation(rotationMatrix);
    } else {
      glm::vec3 centerPoint = CylinderList::calculateCenterPoint();
      for (int i = 0; i < CylinderList::cylinders.size(); ++i) {
        CylinderList::cylinders[i]->changeRotation(rotationMatrix, centerPoint);
      }
    }
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
  float h = entity->getScale().y;
  c.r = entity->getScale().x;
  c.h = h * 2.0l;
  glm::mat4 transformationMatrix = entity->getTransformtationMatrix();
  glm::vec4 A4d = transformationMatrix * glm::vec4(0.0f, h, 0.0f, 1.0f);
  glm::vec4 B4d = transformationMatrix * glm::vec4(0.0f, -h, 0.0f, 1.0f);
  glm::vec3 A = glm::vec3(A4d.x, A4d.y, A4d.z);
  glm::vec3 B = glm::vec3(B4d.x, B4d.y, B4d.z);
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
