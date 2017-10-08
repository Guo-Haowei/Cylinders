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
double MouseManager::previousClickX = -1.0l;
double MouseManager::previousClickY = -1.0l;

int MouseManager::buttons[2] = { 0, 0 };
int MouseManager::currentButtons[2] = { 0, 0 };
int MouseManager::upButtons[2] = { 0, 0 };
int MouseManager::downButtons[2] = { 0, 0 };

MOUSEMODE MouseManager::currentMode = SCENE;

void MouseManager::prepare() {
  DisplayManager::getCursorPos(&lastX, &lastY);
  currentX = lastX;
  currentY = lastY;
}

void MouseManager::beforeUpdate() {
  DisplayManager::getCursorPos(&currentX, &currentY);

  // button event
  upButtons[0] = currentButtons[0] && !buttons[0];
  upButtons[1] = currentButtons[1] && !buttons[1];

  downButtons[0] = !currentButtons[0] && buttons[0];
  downButtons[1] = !currentButtons[1] && buttons[1];

  currentButtons[0] = buttons[0];
  currentButtons[1] = buttons[1];
}

void MouseManager::afterUpdate() {
  lastX = currentX;
  lastY = currentY;
  xScrollOffset = 0;
  yScrollOffset = 0;

  // record previous click
  if (downButtons[0]) {
    previousClickX = currentX;
    previousClickY = currentY;
  }
}

void MouseManager::setMode(MOUSEMODE mode) {
  currentMode = mode;
}

MOUSEMODE MouseManager::getMouseMode() {
  return currentMode;
}

bool MouseManager::buttonUp(int button) {
  return buttons[button] == 0;
}

bool MouseManager::buttonDown(int button) {
  return buttons[button] != 0;
}

bool MouseManager::buttonPressed(int button) {
  return downButtons[button];
}

bool MouseManager::buttonReleased(int button) {
  return upButtons[button];
}

bool MouseManager::doubleClick() {
  return downButtons[0] && previousClickX == currentX && previousClickY == currentY;
}
