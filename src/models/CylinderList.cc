#define _USE_MATH_DEFINES
#include "CylinderList.h"
#include "../common.h"
#include "../maths/Maths.h"
#include <math.h>
#include <iostream>
using std::abs;
using std::cout;
using std::endl;

Entity* CylinderList::selected = nullptr;
vector<Entity*> CylinderList::cylinders;

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

RawModel CylinderList::createUniformCylinder() {
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
    normals.push_back(x1);
    normals.push_back(0);
    normals.push_back(z1);
    normals.push_back(x2);
    normals.push_back(0);
    normals.push_back(z2);
    normals.push_back(x1);
    normals.push_back(0);
    normals.push_back(z1);
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
    normals.push_back(x2);
    normals.push_back(0);
    normals.push_back(z2);
    normals.push_back(x2);
    normals.push_back(0);
    normals.push_back(z2);
    normals.push_back(x1);
    normals.push_back(0);
    normals.push_back(z1);
  }

  return Loader::loadToVAO(vertices, normals);
}
