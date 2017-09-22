#include "MouseManager.h"
#include "../renderEngine/DisplayManager.h"

double MouseManager::lastX;
double MouseManager::lastY;
double MouseManager::currentX;
double MouseManager::currentY;
double MouseManager::xOffset;
double MouseManager::yOffset;
double MouseManager::xScrollOffset;
double MouseManager::yScrollOffset;

void MouseManager::prepare() {
  DisplayManager::getCursorPos(&lastX, &lastY);
  currentX = lastX;
  currentY = lastY;
}

void MouseManager::beforeUpdate() {
  DisplayManager::getCursorPos(&currentX, &currentY);
}

void MouseManager::afterUpdate() {
  lastX = currentX;
  lastY = currentY;
  xScrollOffset = 0;
  yScrollOffset = 0;
}
