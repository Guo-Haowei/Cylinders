#version 400 core

in vec3 position;
in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
  vec4 worldPosition = transformationMatrix * vec4(position, 1.0);
  gl_Position = projectionMatrix * viewMatrix * worldPosition;
  FragPos = vec3(worldPosition);
  Normal = normalize(normal);
  // Normal = vec3(0, 1, 0);
}
