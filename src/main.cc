#include "entities/Camera.h"
#include "entities/Entity.h"
#include "inputManager/KeyboardManager.h"
#include "inputManager/MouseManager.h"
#include "IO/IO.h"
#include "models/CylinderList.h"
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

#if COUNT_FPS
void updateFPSCount(double& previousSecond, int& updates) {
  if (DisplayManager::getTime() - previousSecond < 1.0) {
    return;
  }

  ++previousSecond;
  cout << "FPS is " << updates << endl;
  updates = 0;
}
#endif

int main(int argc, char* argv[]) {

  DisplayManager::createDisplay();
  Loader loader;
  Renderer renderer;
  vector<Entity*> entities;
  RawModel model = CylinderList::createUniformCylinder(loader);
  Camera::updateCameraVectors();
  MouseManager::prepare();

  int fps = 60;
  double currentTime, lastTime = DisplayManager::getTime();
#if COUNT_FPS
  double previousSecond = lastTime;
#endif
  double delta = 0;
  int updates = 0;

  if (argc == 2) {
    IO::read(argv[1], CylinderList::cylinders, model);
  }

  while (!DisplayManager::shouldCloseDisplay()) {
    if (shouldUpdate(currentTime, delta, lastTime, fps)) {
      DisplayManager::prepareDisplay();
      MouseManager::beforeUpdate();
      Camera::update();
      CylinderList::update(model);

      renderer.render(CylinderList::cylinders);

      DisplayManager::updateDisplay();
      MouseManager::afterUpdate();
      KeyboardManager::update();
      ++updates;
    }

#if COUNT_FPS
    updateFPSCount(previousSecond, updates);
#endif
  }
  DisplayManager::cleanDisplay();
  CylinderList::clean();

  return 0;
}
