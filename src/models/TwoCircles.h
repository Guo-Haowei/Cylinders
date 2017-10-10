#pragma once
#include "Loader.h"
#include "RawModel.h"
#include "../entities/Entity.h"
#include <glm/glm.hpp>

class TwoCircles {
public:
  static Entity* upper;
  static Entity* lower;
  static RawModel createUniformUpperCircle();
  static RawModel createUniformLowerCircle();
  static void clean();
};
