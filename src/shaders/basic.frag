#version 400 core

in vec3 FragPos;
in vec3 Normal;

out vec4 out_Color;

uniform vec3 color;

void main() {
  // temp light properties
  vec3 lightPos = vec3(0, 7, -8);
  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
  vec3 lightDir = normalize(lightPos - FragPos);

  float diff = max(dot(Normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * lightColor;

  vec3 result = (ambient + diffuse) * color;
  out_Color = vec4(result, 1.0);
}
