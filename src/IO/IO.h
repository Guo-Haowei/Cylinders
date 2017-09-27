#pragma once
#include "../entities/Entity.h"
#include <vector>

class IO {
  static int fileCount;
public:
  static void write(std::vector<Entity*>& entities);
  static void read(const char* name);
};
