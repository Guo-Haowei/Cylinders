#pragma once
#include "../entities/Entity.h"
#include "../models/RawModel.h"
#include <string>
#include <vector>

class IO {
  static int fileCount;
public:
  static void write(std::vector<Entity*>& entities);
  static void read(std::string name, std::vector<Entity*>& entities, RawModel& model);
};
