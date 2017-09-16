#pragma once
#include "ShaderProgram.h"

class EntityShader: public ShaderProgram {
private:
  static const char* VERTEX_FILE;
  static const char* FRAGMENT_FILE;

  // uniform locations
protected:
  void bindAttributes();
  void getAllUniformLocations();
public:
  EntityShader();
};
