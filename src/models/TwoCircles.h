#pragma once
#include "Loader.h"
#include "RawModel.h"
#include "../entities/Entity.h"
#include <glm/glm.hpp>

class TwoCircles {
private:
  static bool renderCircle;
public:
  static Entity* upper;
  static Entity* lower;
  static Entity* twoCircles;
  static RawModel createUniformUpperCircle();
  static RawModel createUniformLowerCircle();
  static RawModel createUniformTwoCircles();
  static void clean();
};
