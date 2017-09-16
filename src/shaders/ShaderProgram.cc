#define GLEW_STATIC
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void ShaderProgram::init(const char*vertexFileName, const char* fragmentFileName) {
  vertexShaderID = loadShader(vertexFileName, GL_VERTEX_SHADER);
  fragmentShaderID = loadShader(fragmentFileName, GL_FRAGMENT_SHADER);
  programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  bindAttributes();
  glLinkProgram(programID);

  int success;
  char infoLog[512];
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(programID, 512, NULL, infoLog);
    std::cout << "==================================================\n";
    std::cout << "ERROR::SHADER: Failed to link program\n\n" << infoLog;
    std::cout << "\n==================================================\n";
  }

  getAllUniformLocations();
}

void ShaderProgram::start() {
  glUseProgram(programID);
}

void ShaderProgram::stop() {
  glUseProgram(0);
}

void ShaderProgram::clean() {
  stop();
  glDetachShader(programID, vertexShaderID);
  glDetachShader(programID, fragmentShaderID);
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
  glDeleteProgram(programID);
}

void ShaderProgram::bindAttribute(unsigned int attribute, const char* variable) {
  glBindAttribLocation(programID, attribute, variable);
}

int ShaderProgram::getUniformLocation(const char* uniformName) {
  return glGetUniformLocation(programID, uniformName);
}

void ShaderProgram::loadFloat(int location, float value) {
  glUniform1f(location, value);
}

unsigned int ShaderProgram::loadShader(const char* file, unsigned int type) {
  try {
    std::fstream fs(file);
    if (!fs.good()) {
      throw std::exception();
    }

    std::stringstream ss;
    ss << fs.rdbuf();
    std::string shaderSourceString = ss.str();
    const char* shaderSource = shaderSourceString.c_str();
    unsigned int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
      std::cout << "==================================================\n";
      std::cout << "ERROR::SHADER: Failed to compile shader\n\n" << infoLog;
      std::cout << "\n==================================================\n";
    }

    return shaderID;
  } catch (std::exception &e) {
    std::cout << "==================================================\n";
    std::cout << "ERROR::SHADER: Failed to read file\n";
    std::cout << "\n==================================================\n";

    return 0;
  }
}
