#pragma once
#include <glm/glm.hpp>

namespace Maths {
  glm::mat4 calculateRotationMatrix(glm::vec3 P0, glm::vec3 P1, glm::vec3 a);

  bool isNaNVector(glm::vec3 vec);
  bool isNaNMatrix(glm::mat4 mat);
};
