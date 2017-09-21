#include "inputManager/KeyboardManager.h"
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
  std::cout << "FPS is " << updates << std::endl;
  updates = 0;
}

int main() {

  DisplayManager::createDisplay();
  Loader loader;
  Renderer renderer;
  std::vector<Entity*> entities;
  RawModel model = Cylinder::createUniformCylinder(loader);
  Entity entity1(model, glm::vec3(0.4, 0.7, 0.6), glm::vec3(0, 0, -5));
  Entity entity2(model, glm::vec3(0.5, 0.2, 0.3), glm::vec3(0, 3, -7), glm::vec3(5, 6, -7));
  entities.push_back(&entity1);
  // entities.push_back(&entity2);
  Camera camera;

  int fps = 60;
  double currentTime, lastTime = DisplayManager::getTime(), previousSecond = lastTime;
  double delta = 0;
  int updates = 0;

  while (!DisplayManager::shouldCloseDisplay()) {
    if (shouldUpdate(currentTime, delta, lastTime, fps)) {
      entity1.changeRotation(0.01, 0.0f, 0.01f);

      DisplayManager::prepareDisplay();
      camera.update();

      renderer.render(entities, camera);

      DisplayManager::updateDisplay();
      KeyboardManager::update();
      ++updates;
    }

    updateFPSCount(previousSecond, updates);
  }
  DisplayManager::cleanDisplay();

  return 0;
}
