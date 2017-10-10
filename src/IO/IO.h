#pragma once
#include "../entities/Entity.h"
#include "../models/RawModel.h"
#include <string>
#include <vector>

class IO {
public:
  static void write(std::vector<Entity*>& entities, std::string name = "cylinders");
  static void read(std::vector<Entity*>& entities, RawModel& model, std::string name = "../cylinders.txt");
  static void manual();
};
