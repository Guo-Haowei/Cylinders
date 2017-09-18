#define _USE_MATH_DEFINES
#include "Cylinder.h"
#include <math.h>
#include <vector>
using std::vector;

RawModel Cylinder::createUniformCylinder(Loader& loader) {
  vector<float> positions;
  vector<float> normals;
  vector<unsigned int> indices;
  for (int i = 0; i < 360; i += 2) {
    float x = sin(i * M_PI / 180);
    float z = cos(i * M_PI / 180);
    positions.push_back(x);
    positions.push_back(1.0f);
    positions.push_back(z);
    positions.push_back(x);
    positions.push_back(-1.0f);
    positions.push_back(z);
  }
  int size = positions.size() / 3;
  positions.push_back(0.0f);
  positions.push_back(1.0f);
  positions.push_back(0.0f);
  positions.push_back(0.0f);
  positions.push_back(-1.0f);
  positions.push_back(0.0f);

  // indices and normals
  // faces
  for (int i = 0; i + 2 < size; i += 2) {
    indices.push_back(size);
    indices.push_back(i);
    indices.push_back(i + 2);
    indices.push_back(size + 1);
    indices.push_back(i + 1);
    indices.push_back(i + 3);
  }
  indices.push_back(size);
  indices.push_back(size - 2);
  indices.push_back(0);
  indices.push_back(size + 1);
  indices.push_back(1);
  indices.push_back(size - 1);

  // side
  for (int i = 0; i + 2 < size; i += 2) {
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
    indices.push_back(i + 3);
  }

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(size - 1);
  indices.push_back(0);
  indices.push_back(size - 1);
  indices.push_back(size - 2);

  return loader.loadToVAO(positions, normals, indices);
}
