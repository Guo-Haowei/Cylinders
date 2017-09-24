#pragma once

enum MOUSEMODE {
  SCENE = 0,
  OBJECT
};

enum BUTTONS {
  LEFT_BUTTON = 0,
  RIGHT_BUTTON
};

class MouseManager {
private:
  static MOUSEMODE currentMode;

public:
  static double lastX;
  static double lastY;
  static double currentX;
  static double currentY;
  static double xOffset;
  static double yOffset;
  static double xScrollOffset;
  static double yScrollOffset;

  static int buttons[2];
  static int currentButtons[2];
  static int upButtons[2];
  static int downButtons[2];

  static void prepare();
  static void beforeUpdate();
  static void afterUpdate();

  static void setMode(MOUSEMODE mode);
  static MOUSEMODE getMouseMode();

  static bool buttonUp(int button);
  static bool buttonDown(int button);
  static bool buttonPressed(int button);
  static bool buttonReleased(int button);
};
