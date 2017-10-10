#pragma once
#include "Loader.h"
#include "RawModel.h"
#include "../entities/Entity.h"
#include <glm/glm.hpp>
#include <vector>
using std::vector;

class CylinderList {
public:
  static Entity* selected;
  static vector<Entity*> cylinders;
  static glm::vec3 calculateCenterPoint();
  static void clean();

  static RawModel createUniformCylinder();
};
