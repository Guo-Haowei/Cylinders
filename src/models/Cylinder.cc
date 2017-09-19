#define _USE_MATH_DEFINES
#include "Cylinder.h"
#include <math.h>
#include <iostream>
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
    normals.push_back(x);
    normals.push_back(1.0f);
    normals.push_back(z);
    normals.push_back(x);
    normals.push_back(-1.0f);
    normals.push_back(z);
  }
  int size = positions.size() / 3;
  positions.push_back(0.0f);
  positions.push_back(1.0f);
  positions.push_back(0.0f);
  positions.push_back(0.0f);
  positions.push_back(-1.0f);
  positions.push_back(0.0f);
  normals.push_back(0.0f);
  normals.push_back(1.0f);
  normals.push_back(0.0f);
  normals.push_back(0.0f);
  normals.push_back(-1.0f);
  normals.push_back(0.0f);

  // indices and normals
  // faces
  for (int i = 0; i + 2 < size; i += 2) {
    // upper face
    indices.push_back(size);
    indices.push_back(i);
    indices.push_back(i + 2);
    // upper normal


    // lower face
    indices.push_back(size + 1);
    indices.push_back(i + 1);
    indices.push_back(i + 3);
    // lower normal

  }
  // upper face
  indices.push_back(size);
  indices.push_back(size - 2);
  indices.push_back(0);
  // upper normal

  // lower face
  indices.push_back(size + 1);
  indices.push_back(1);
  indices.push_back(size - 1);
  // lower normal


  // side
  for (int i = 0; i + 2 < size; i += 2) {
    // face
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
    indices.push_back(i + 3);
    // normal
    float x = (positions[3 * i] + positions[3 * i + 6]) / 2;
    float z = (positions[3 * i + 2] + positions[3 * i + 8]) / 2;
  }
  // face
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(size - 1);
  indices.push_back(0);
  indices.push_back(size - 1);
  indices.push_back(size - 2);
  // normal
  float x = (positions[0] + positions[3 * size - 6]) / 2;
  float z = (positions[2] + positions[3 * size - 4]) / 2;

  std::cout << positions.size() << std::endl;
  std::cout << normals.size() << std::endl;
  std::cout << indices.size() << std::endl;

  return loader.loadToVAO(positions, normals, indices);
}
