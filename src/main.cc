#include "inputManager/KeyboardManager.h"
#include "inputManager/MouseManager.h"
#include "entities/Camera.h"
#include "entities/Entity.h"
#include "models/Cylinder.h"
#include "models/Loader.h"
#include "models/RawModel.h"
#include "renderEngine/DisplayManager.h"
#include "renderEngine/Renderer.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

bool shouldUpdate(double& currentTime, double& delta, double& lastTime, const int fps) {
  currentTime = DisplayManager::getTime();
  delta += currentTime - lastTime;
  lastTime  = currentTime;
  if (delta >= 1.0 /fps) {
    delta -= 1.0 /fps;
    return true;
  } else {
    return false;
  }
}

void updateFPSCount(double& previousSecond, int& updates) {
  if (DisplayManager::getTime() - previousSecond < 1.0) {
    return;
  }

  ++previousSecond;
  cout << "FPS is " << updates << endl;
  updates = 0;
}

int main() {

  DisplayManager::createDisplay();
  Loader loader;
  Renderer renderer;
  vector<Entity*> entities;
  RawModel model = Cylinder::createUniformCylinder(loader);
  Camera::updateCameraVectors();
  MouseManager::prepare();

  int fps = 60;
  double currentTime, lastTime = DisplayManager::getTime(), previousSecond = lastTime;
  double delta = 0;
  int updates = 0;

  while (!DisplayManager::shouldCloseDisplay()) {
    if (shouldUpdate(currentTime, delta, lastTime, fps)) {
      DisplayManager::prepareDisplay();
      MouseManager::beforeUpdate();
      Camera::update();
      Cylinder::update(model);

      renderer.render(Cylinder::cylinders);

      DisplayManager::updateDisplay();
      MouseManager::afterUpdate();
      KeyboardManager::update();
      ++updates;
    }

    updateFPSCount(previousSecond, updates);
  }
  DisplayManager::cleanDisplay();
  Cylinder::clean();

  return 0;
}
