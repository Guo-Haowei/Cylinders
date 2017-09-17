#include "entities/Entity.h"
#include "models/Loader.h"
#include "models/RawModel.h"
#include "renderEngine/DisplayManager.h"
#include "renderEngine/Renderer.h"
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

  std::vector<float> vertices;
  vertices.push_back(-0.5f);
  vertices.push_back(-0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(-0.5f);
  vertices.push_back(0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(0.5f);
  vertices.push_back(0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(0.5f);
  vertices.push_back(-0.5f);
  vertices.push_back(0.0f);
  std::vector<float> normals;
  std::vector<unsigned int> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);

  DisplayManager::createDisplay();
  Loader loader;
  Renderer renderer;
  std::vector<Entity*> entities;
  RawModel model = loader.loadToVAO(vertices, normals, indices);
  Entity entity(model);
  entities.push_back(&entity);

  int fps = 60;
  double currentTime, lastTime = DisplayManager::getTime(), previousSecond = lastTime;
  double delta = 0;
  int updates = 0;

  while (!DisplayManager::shouldCloseDisplay()) {
    if (shouldUpdate(currentTime, delta, lastTime, fps)) {
      DisplayManager::prepareDisplay();

      renderer.render(entities);

      DisplayManager::updateDisplay();
      ++updates;
    }

    updateFPSCount(previousSecond, updates);
  }
  DisplayManager::cleanDisplay();

  return 0;
}
