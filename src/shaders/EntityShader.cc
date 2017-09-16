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

}
