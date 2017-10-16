#include "Maths.h"
#include <math.h>
#include <iostream>
using std::cout;
using std::abs;

glm::mat4 Maths::calculateRotationMatrix(float cosTheta, float sinTheta, glm::vec3 a) {
  float yz = sqrt(a.y * a.y + a.z * a.z);
  glm::mat4 Rx = glm::mat4(1.0f), RxInverse = glm::mat4(1.0f), Ry = glm::mat4(1.0f), RyInverse = glm::mat4(1.0f), Rz = glm::mat4(1.0f);
  float sinTheta1 = a.y / yz, cosTheta1 = a.z / yz;
  Rx[1].y = cosTheta1; Rx[1].z = sinTheta1; Rx[2].y = -sinTheta1; Rx[2].z = cosTheta1;
  RxInverse[1].y = cosTheta1; RxInverse[1].z = -sinTheta1; RxInverse[2].y = sinTheta1; RxInverse[2].z = cosTheta1;
  float sinTheta2 = -a.x, cosTheta2 = yz;
  Ry[0].x = cosTheta2; Ry[0].z = -sinTheta2; Ry[2].x = sinTheta2; Ry[2].z = cosTheta2;
  RyInverse[0].x = cosTheta2; RyInverse[0].z = sinTheta2; RyInverse[2].x = -sinTheta2; RyInverse[2].z = cosTheta2;

  // angle theta
  Rz[0].x = cosTheta; Rz[0].y = sinTheta; Rz[1].x = -sinTheta; Rz[1].y = cosTheta;

  return RxInverse * RyInverse * Rz * Ry * Rx;
}

bool Maths::isNaNVector(glm::vec3 vec) {
  return std::isnan(vec.x) || std::isnan(vec.y) || std::isnan(vec.z);
}

bool Maths::isNaNMatrix(glm::mat4 mat) {
  bool isNaN = false;
  for (int i = 0; i < 4; ++i) {
    isNaN = isNaN || std::isnan(mat[i].x) || std::isnan(mat[i].y) || std::isnan(mat[i].z) || std::isnan(mat[i].w);
  }
  return isNaN;
}
