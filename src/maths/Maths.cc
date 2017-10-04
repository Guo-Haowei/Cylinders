#include "Maths.h"
#include <math.h>
using std::abs;

glm::mat4 Maths::calculateRotationMatrix(glm::vec3 P0, glm::vec3 P1, glm::vec3 a) {
  // rotation degree
  float cosTheta = glm::dot(P0, P1) / (glm::length(P0) * glm::length(P1));
  float sinTheta = sqrt(1 - cosTheta * cosTheta);

  float xAbs = abs(a.x), yAbs = abs(a.y), zAbs = abs(a.z);
  // project onto xz plane
  float XZ = sqrt(a.x * a.x + a.z * a.z);
  float cosPhi = a.x / XZ;
  float sinPhi = a.z / XZ;

  glm::mat4 Ry_phi(1.0f);
  Ry_phi[0].x = cosPhi; Ry_phi[0].z = sinPhi;
  Ry_phi[2].x = -sinPhi; Ry_phi[2].z = cosPhi;

  glm::mat4 Ryphi(1.0f);
  Ryphi[0].x = cosPhi; Ryphi[0].z = -sinPhi;
  Ryphi[2].x = sinPhi; Ryphi[2].z = cosPhi;

  float cosPsi = XZ;
  float sinPsi = a.y;

  glm::mat4 Rz_psi(1.0f);
  Rz_psi[0].x = cosPsi; Rz_psi[0].y = sinPsi;
  Rz_psi[1].x = -sinPsi; Rz_psi[1].y = cosPsi;

  glm::mat4 Rzpsi(1.0f);
  Rzpsi[0].x = cosPsi; Rzpsi[0].y = -sinPsi;
  Rzpsi[1].x = sinPsi; Rzpsi[1].y = cosPsi;

  glm::mat4 RxTheta(1.0f);
  RxTheta[1].y = cosTheta; RxTheta[1].z = -sinTheta;
  RxTheta[2].y = sinTheta; RxTheta[2].z = cosTheta;

  return Ryphi * Rzpsi * RxTheta * Rz_psi * Ry_phi;
}
