#include "ColorPickShader.h"

ColorPickShader::ColorPickShader() {
  const char* VERTEX_FILE = "../src/shaders/colorPick.vert";
  const char* FRAGMENT_FILE = "../src/shaders/colorPick.frag";
  init(VERTEX_FILE, FRAGMENT_FILE);
}

void ColorPickShader::bindAttributes() {
  bindAttribute(0, "position");
}

void ColorPickShader::loadColor(int id) {
  int r = (id & 0x000000ff) >> 0;
  int g = (id & 0x0000ff00) >> 8;
  int b = (id & 0x00ff0000) >> 16;
  loadVector3f(location_color, glm::vec3(r/255.0f, g/255.0f, b/255.0f));
}
