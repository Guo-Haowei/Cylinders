#pragma once

class ShaderProgram {
private:
  unsigned int programID;
  unsigned int vertexShaderID;
  unsigned int fragmentShaderID;

  static unsigned int loadShader(const char* file, unsigned int type);
protected:
  virtual void bindAttributes() = 0;
  void bindAttribute(unsigned int attribute, const char* variable);
  virtual void getAllUniformLocations() = 0;
  int getUniformLocation(const char* uniformName);
  void loadFloat(int location, float value);
  // void loadVector3f(int location, Vector3f vec);
  // void loadMatrix4f(int location, Matrix4f mat);
public:
  void init(const char* vertexFileName, const char* fragmentFileName);
  void start();
  void stop();
  void clean();
};
