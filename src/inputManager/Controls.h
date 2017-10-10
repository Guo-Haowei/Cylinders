#pragma once
#include "../models/Loader.h"
#include "../models/RawModel.h"
#include <glm/glm.hpp>
#include <vector>
using std::vector;

class Controls {
public:
  static void update(RawModel& model);
};
