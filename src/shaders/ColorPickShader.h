#pragma once
#include "EntityShader.h"

class ColorPickShader: public EntityShader {
public:
  ColorPickShader();

  void bindAttributes();
  void loadColor(int id);
  void loadColor(glm::vec3 color);
};
