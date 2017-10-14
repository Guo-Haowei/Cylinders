#include "Maths.h"
#include <math.h>
#include <iostream>
using std::cout;
using std::abs;

glm::mat4 Maths::calculateRotationMatrix(glm::vec3 P0, glm::vec3 P1, glm::vec3 a) {
  // rotation degree
  //
  // // project onto xz plane
  // float XZ = sqrt(a.x * a.x + a.z * a.z);
  // float cosPhi = a.x / XZ;
  // float sinPhi = a.z / XZ;
  //
  // glm::mat4 Ry_phi(1.0f);
  // Ry_phi[0].x = cosPhi; Ry_phi[0].z = sinPhi;
  // Ry_phi[2].x = -sinPhi; Ry_phi[2].z = cosPhi;
  //
  // glm::mat4 Ryphi(1.0f);
  // Ryphi[0].x = cosPhi; Ryphi[0].z = -sinPhi;
  // Ryphi[2].x = sinPhi; Ryphi[2].z = cosPhi;
  //
  // float cosPsi = XZ;
  // float sinPsi = a.y;
  //
  // glm::mat4 Rz_psi(1.0f);
  // Rz_psi[0].x = cosPsi; Rz_psi[0].y = sinPsi;
  // Rz_psi[1].x = -sinPsi; Rz_psi[1].y = cosPsi;
  //
  // glm::mat4 Rzpsi(1.0f);
  // Rzpsi[0].x = cosPsi; Rzpsi[0].y = -sinPsi;
  // Rzpsi[1].x = sinPsi; Rzpsi[1].y = cosPsi;
  //
  // glm::mat4 RxTheta(1.0f);
  // RxTheta[1].y = cosTheta; RxTheta[1].z = -sinTheta;
  // RxTheta[2].y = sinTheta; RxTheta[2].z = cosTheta;
  //
  // return Ryphi * Rzpsi * RxTheta * Rz_psi * Ry_phi;
  float yz = sqrt(a.y * a.y + a.z * a.z);
  glm::mat4 Rx = glm::mat4(1.0f), RxInverse = glm::mat4(1.0f), Ry = glm::mat4(1.0f), RyInverse = glm::mat4(1.0f), Rz = glm::mat4(1.0f);
  float sinTheta1 = a.y / yz, cosTheta1 = a.z / yz;
  Rx[1].y = cosTheta1; Rx[1].z = -sinTheta1; Rx[2].y = sinTheta1; Rx[2].z = cosTheta1;
  RxInverse[1].y = cosTheta1; RxInverse[1].z = sinTheta1; RxInverse[2].y = -sinTheta1; RxInverse[2].z = cosTheta1;
  float sinTheta2 = -a.x, cosTheta2 = yz;
  Ry[0].x = cosTheta2; Ry[0].z = sinTheta2; Ry[2].x = -sinTheta2; Ry[2].z = cosTheta2;
  RyInverse[0].x = cosTheta2; RyInverse[0].z = -sinTheta2; RyInverse[2].x = sinTheta2; RyInverse[2].z = cosTheta2;

  // angle theta
  float cosTheta = glm::dot(P0, P1) / (glm::length(P0) * glm::length(P1));
  float sinTheta = -sqrt(1 - cosTheta * cosTheta);
  Rz[0].x = cosTheta; Rz[0].y = -sinTheta; Rz[1].x = sinTheta; Rz[1].y = cosTheta;

  return RxInverse * RyInverse * Rz * Ry * Rx;
}

bool Maths::isNaNMatrix(glm::mat4 mat) {
  bool isNaN = false;
  for (int i = 0; i < 4; ++i) {
    isNaN = isNaN || std::isnan(mat[i].x) || std::isnan(mat[i].y) || std::isnan(mat[i].z) || std::isnan(mat[i].w);
  }
  return isNaN;
}
