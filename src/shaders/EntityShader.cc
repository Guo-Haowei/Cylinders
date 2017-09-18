#include "EntityShader.h"

const char* EntityShader::VERTEX_FILE = "../src/shaders/basic.vert";
const char* EntityShader::FRAGMENT_FILE = "../src/shaders/basic.frag";

EntityShader::EntityShader() {
  init(VERTEX_FILE, FRAGMENT_FILE);
}

void EntityShader::bindAttributes() {
  bindAttribute(0, "position");
  bindAttribute(1, "normal");
}

void EntityShader::getAllUniformLocations() {
  location_transformationMatrix = getUniformLocation("transformationMatrix");
  location_projectionMatrix = getUniformLocation("projectionMatrix");
  location_viewMatrix = getUniformLocation("viewMatrix");
  location_color = getUniformLocation("color");
}

void EntityShader::loadTransformationMatrx(glm::mat4 mat) {
  loadMatrix4f(location_transformationMatrix, mat);
}

void EntityShader::loadProjectionMatrix(glm::mat4 mat) {
  loadMatrix4f(location_projectionMatrix, mat);
}

void EntityShader::loadViewMatrix(Camera& camera) {
  loadMatrix4f(location_viewMatrix, camera.getViewMatrix());
}

void EntityShader::loadColor(glm::vec3 color) {
  loadVector3f(location_color, color);
}
