#pragma once
#include <glm/glm.hpp>

namespace Maths {
  glm::mat4 calculateRotationMatrix(float cosTheta, float sinTheta, glm::vec3 a);

  bool isNaNVector(glm::vec3 vec);
  bool isNaNMatrix(glm::mat4 mat);
};
