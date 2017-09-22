#pragma once

enum MOUSEMODE {
  SCENE = 0,
  OBJECT
};

class MouseManager {
public:
  static double lastX;
  static double lastY;
  static double currentX;
  static double currentY;
  static double xOffset;
  static double yOffset;
  static double xScrollOffset;
  static double yScrollOffset;

  static void prepare();
  static void beforeUpdate();
  static void afterUpdate();
};
