#pragma once
#include "../entities/Entity.h"
#include "../models/RawModel.h"
#include "../shaders/EntityShader.h"
#include "../shaders/ColorPickShader.h"
#include <vector>
using std::vector;

enum SHADER {
  ENTITY,
  COLORPICK
};

class Renderer {
private:
  EntityShader entityShader;
  ColorPickShader colorPickShader;
public:
  Renderer();
  ~Renderer();

  void render(vector<Entity*> entities);
  void renderEntity(Entity* entity, SHADER type = ENTITY);
  void prepare();
  void prepareModel(Entity* entity);
  void unbindModel();
};
