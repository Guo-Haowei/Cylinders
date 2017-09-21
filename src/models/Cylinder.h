#pragma once
#include "Loader.h"
#include "RawModel.h"
#include "../entities/Entity.h"
#include <vector>
using std::vector;

class Cylinder {
private:
  static Entity* selected;
public:
  static vector<Entity*> cylinders;
  static void update(RawModel& model);
  static RawModel createUniformCylinder(Loader& loader);
  static void clean();
};
