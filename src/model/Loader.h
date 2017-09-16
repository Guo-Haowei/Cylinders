#pragma once
#include "RawModel.h"
#include <vector>
using std::vector;

class Loader {
private:
  vector<unsigned int> vaos;
  vector<unsigned int> vbos;

  unsigned int createVAO();
  void unbindVAO();
  void storeDataInAttributeList(unsigned int attrubuteNumber, int coordinateSize, vector<float> data);
  void bindIndicesBuffer(vector<unsigned int> indices);

public:
  ~Loader();

  RawModel loadToVAO(vector<float> positions, vector<float> normals, vector<unsigned int> indices);
};
