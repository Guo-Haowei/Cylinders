#pragma once
#include "ShaderProgram.h"

class EntityShader: public ShaderProgram {
private:
  static const char* VERTEX_FILE;
  static const char* FRAGMENT_FILE;

  // uniform locations
  int location_transformationMatrix;
  int location_projectionMatrix;
  int location_viewMatrix;
protected:
  void bindAttributes();
  void getAllUniformLocations();
public:
  EntityShader();

  void loadTransformationMatrx(glm::mat4 mat);
  void loadProjectionMatrix(glm::mat4 mat);
  void loadViewMatrix(glm::mat4 mat);
};
