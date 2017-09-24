#pragma once
#include "../entities/Entity.h"
#include "../models/RawModel.h"
#include "../shaders/EntityShader.h"
#include <vector>
using std::vector;

class Renderer {
private:
  EntityShader entityShader;
public:
  Renderer();

  void render(vector<Entity*> entities);
  void renderEntity(Entity* entity);
  void prepare();
  void prepareModel(Entity* entity);
  void unbindModel();
};
