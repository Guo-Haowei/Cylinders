#pragma once
#include "Loader.h"
#include "RawModel.h"
#include "../entities/Entity.h"
#include <vector>
using std::vector;

class CylinderList {
private:
  static Entity* selected;
  static int selectedEntry;
  static void output();
public:
  static vector<Entity*> cylinders;
  static void update(RawModel& model);
  static void clean();
};
