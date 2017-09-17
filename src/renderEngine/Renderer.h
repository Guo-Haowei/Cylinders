#pragma once
#include "../entities/Camera.h"
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

  void render(vector<Entity*> entities, Camera& camera);
  void renderEntity(Entity* entity);
  void prepare();
  void prepareModel(Entity* entity);
  void unbindModel();
};
