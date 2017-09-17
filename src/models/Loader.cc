#define GLEW_STATIC
#include "Loader.h"
#include <GL/glew.h>

RawModel Loader::loadToVAO(vector<float> positions, vector<float> normals, vector<unsigned int> indices) {
  unsigned int vaoID = createVAO();
  bindIndicesBuffer(indices);
  storeDataInAttributeList(0, 3, positions);
  storeDataInAttributeList(1, 3, normals);
  unbindVAO();
  return RawModel(vaoID, indices.size());
}

Loader::~Loader() {
  for (auto& vao: vaos) {
    glDeleteVertexArrays(1, &vao);
  }
  for (auto& vbo: vbos) {
    glDeleteBuffers(1, &vbo);
  }
}

unsigned int Loader::createVAO() {
  unsigned int vaoID;
  glGenVertexArrays(1, &vaoID);
  vaos.push_back(vaoID);
  glBindVertexArray(vaoID);
  return vaoID;
}

void Loader::unbindVAO() {
  glBindVertexArray(0);
}

void Loader::storeDataInAttributeList(unsigned int attrubuteNumber, int coordinateSize, vector<float> data) {
  unsigned int vboID;
  glGenBuffers(1, &vboID);
  vbos.push_back(vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.front(), GL_STATIC_DRAW);
  glVertexAttribPointer(attrubuteNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIndicesBuffer(vector<unsigned int> indices) {
  unsigned int vboID;
  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
