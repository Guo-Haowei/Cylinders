#pragma once
#include "Loader.h"
#include "RawModel.h"
#include "../entities/Entity.h"
#include <glm/glm.hpp>

class TwoCircles {
public:
  static Entity* twoCircles;
  static RawModel createUniformTwoCircles();
};
