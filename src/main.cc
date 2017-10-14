#include "entities/Camera.h"
#include "entities/Entity.h"
#include "inputManager/Controls.h"
#include "inputManager/KeyboardManager.h"
#include "inputManager/MouseManager.h"
#include "IO/IO.h"
#include "models/CylinderList.h"
#include "models/Loader.h"
#include "models/RawModel.h"
#include "models/TwoCircles.h"
#include "renderEngine/DisplayManager.h"
#include "renderEngine/Renderer.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string.h>
using std::cout;
using std::cerr;
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
  DisplayManager::setTitle(("Cylinders -- FPS: " + std::to_string(updates)).c_str());
  updates = 0;
}

int main(int argc, char* argv[]) {

  DisplayManager::createDisplay();
  Renderer renderer;
  vector<Entity*> entities;
  RawModel model = CylinderList::createUniformCylinder();
  RawModel circle1 = TwoCircles::createUniformUpperCircle();
  RawModel circle2 = TwoCircles::createUniformLowerCircle();
  glm::vec3 borderColor = glm::vec3(1.0f, 1.0f, 0.0f);
  TwoCircles::upper = new Entity(circle1, borderColor);
  TwoCircles::lower = new Entity(circle2, borderColor);
  Camera::updateCameraVectors();
  MouseManager::prepare();

  int fps = 30;
  double currentTime, lastTime = DisplayManager::getTime();
  double previousSecond = lastTime;
  double delta = 0;
  int updates = 0;

  if (argc == 2 || argc == 3) {
    if (strcmp(argv[1], "-r")) {
      cerr << "Invalid argument\n";
    } else if (argc == 2) {
      IO::read(CylinderList::cylinders, model);
    } else {
      IO::read(CylinderList::cylinders, model, std::string(argv[2]));
    }
  }

  while (!DisplayManager::shouldCloseDisplay()) {
    if (shouldUpdate(currentTime, delta, lastTime, fps)) {
      DisplayManager::prepareDisplay();
      MouseManager::beforeUpdate();
      Camera::update();
      Controls::update(model);

      renderer.render(CylinderList::cylinders);

      DisplayManager::updateDisplay();
      MouseManager::afterUpdate();
      KeyboardManager::update();
      ++updates;
    }

    updateFPSCount(previousSecond, updates);
  }
  DisplayManager::cleanDisplay();
  CylinderList::clean();
  TwoCircles::clean();
  Loader::clean();

  return 0;
}
