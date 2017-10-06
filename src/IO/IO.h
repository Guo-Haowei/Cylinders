#pragma once
#include "../entities/Entity.h"
#include "../models/RawModel.h"
#include <string>
#include <vector>

class IO {
public:
  static void write(std::vector<Entity*>& entities, std::string name = "cylnders");
  static void read(std::vector<Entity*>& entities, RawModel& model);
};
