#version 400 core

out vec4 out_Color;

uniform vec3 color;

void main() {
  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * lightColor;

  vec3 result = ambient * color;
  out_Color = vec4(result, 1.0);
}
