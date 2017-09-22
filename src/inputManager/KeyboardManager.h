#pragma once
#include <set>
using std::set;

namespace KEYS {
  const int KEY_0 = 48;
  const int KEY_1 = 49;
  const int KEY_2 = 50;
  const int KEY_3 = 51;
  const int KEY_4 = 52;
  const int KEY_5 = 53;
  const int KEY_6 = 54;
  const int KEY_7 = 55;
  const int KEY_8 = 56;
  const int KEY_9 = 57;
  const int KEY_A = 65;
  const int KEY_B = 66;
  const int KEY_C = 67;
  const int KEY_D = 68;
  const int KEY_E = 69;
  const int KEY_F = 70;
  const int KEY_G = 71;
  const int KEY_H = 72;
  const int KEY_I = 73;
  const int KEY_J = 74;
  const int KEY_K = 75;
  const int KEY_L = 76;
  const int KEY_M = 77;
  const int KEY_N = 78;
  const int KEY_O = 79;
  const int KEY_P = 80;
  const int KEY_Q = 81;
  const int KEY_R = 82;
  const int KEY_S = 83;
  const int KEY_T = 84;
  const int KEY_U = 85;
  const int KEY_V = 86;
  const int KEY_W = 87;
  const int KEY_X = 88;
  const int KEY_Y = 89;
  const int KEY_Z = 90;
  const int KEY_RIGHT = 262;
  const int KEY_LEFT = 263;
  const int KEY_DOWN = 264;
  const int KEY_UP = 265;
  const int KEY_TAB = 258;

  const int KEY_LEFT_SQUARE_BRACKET = 91;
  const int KEY_RIGHT_SQUARE_BRACKET = 93;
};

class KeyboardManager {
private:
  static int keys[1024];
  static set<int> currentKeys;
  static set<int> downKeys;
  static set<int> upKeys;
public:
  static void update();
  static void setKeyDown(int key);
  static void setKeyUp(int key);
  static bool isKeyUp(int key);
  static bool isKeyDown(int key);
  static bool isKeyPressed(int key);
  static bool isKeyReleased(int key);
};
