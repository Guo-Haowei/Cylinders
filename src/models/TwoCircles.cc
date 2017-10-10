#include "TwoCircles.h"
#include <vector>
using std::vector;

Entity* TwoCircles::upper = nullptr;
Entity* TwoCircles::lower = nullptr;

RawModel TwoCircles::createUniformUpperCircle() {
  vector<float> vertices;
  vector<float> normals;

  for (int a = 2; a <= 360; a += 2) {
    float x1 = sin(a * M_PI / 180);
    float z1 = cos(a * M_PI / 180);
    float x2 = sin((a + 2) * M_PI / 180);
    float z2 = cos((a + 2) * M_PI / 180);
    // top surface
    vertices.push_back(x1);
    vertices.push_back(1.0f);
    vertices.push_back(z1);
    vertices.push_back(x2);
    vertices.push_back(1.0f);
    vertices.push_back(z2);
    // top normals
    normals.push_back(0.0f);
  }

  return Loader::loadToVAO(vertices, normals);
}

RawModel TwoCircles::createUniformLowerCircle() {
  vector<float> vertices;
  vector<float> normals;

  for (int a = 2; a <= 360; a += 2) {
    float x1 = sin(a * M_PI / 180);
    float z1 = cos(a * M_PI / 180);
    float x2 = sin((a + 2) * M_PI / 180);
    float z2 = cos((a + 2) * M_PI / 180);
    // top surface
    vertices.push_back(x1);
    vertices.push_back(-1.0f);
    vertices.push_back(z1);
    vertices.push_back(x2);
    vertices.push_back(-1.0f);
    vertices.push_back(z2);
    // top normals
    normals.push_back(0.0f);
  }

  return Loader::loadToVAO(vertices, normals);
}

void TwoCircles::clean() {
  if (upper)
    delete upper;
  if (lower)
    delete lower;
}
