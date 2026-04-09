#pragma once

#include <cstddef>
#include <map>

#include "raylib.h"
class GamepadControlProxy {
 public:
  GamepadControlProxy() {}
  ~GamepadControlProxy() {}

  void step() {
    gamepad_left_stick_x =
        GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X);
    gamepad_left_stick_y =
        GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_Y);
    gamepad_right_stick_x =
        GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_RIGHT_X);
    gamepad_right_stick_y =
        GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_RIGHT_Y);
    gamepad_left_trigger =
        GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_TRIGGER);
    gamepad_right_trigger =
        GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_RIGHT_TRIGGER);

    for (size_t i = 0; i < 20; i++) {
      gamepad_inputs[i] = IsGamepadButtonDown(GAMEPAD_ID, i);
    }

    joystick_left_stick_x =
        GetGamepadAxisMovement(JOYSTICK_ID, GAMEPAD_AXIS_LEFT_X);
    joystick_left_stick_y =
        GetGamepadAxisMovement(JOYSTICK_ID, GAMEPAD_AXIS_LEFT_Y);
    joystick_right_stick_x =
        GetGamepadAxisMovement(JOYSTICK_ID, GAMEPAD_AXIS_RIGHT_X);
    joystick_right_stick_y =
        GetGamepadAxisMovement(JOYSTICK_ID, GAMEPAD_AXIS_RIGHT_Y);
    joystick_left_trigger =
        GetGamepadAxisMovement(JOYSTICK_ID, GAMEPAD_AXIS_LEFT_TRIGGER);
    joystick_right_trigger =
        GetGamepadAxisMovement(JOYSTICK_ID, GAMEPAD_AXIS_RIGHT_TRIGGER);

    for (size_t i = 0; i < 20; i++) {
      joystick_inputs[i] = IsGamepadButtonDown(JOYSTICK_ID, i);
    }
  }

  std::map<size_t, bool> gamepad_inputs;

  float gamepad_left_stick_x = 0;
  float gamepad_left_stick_y = 0;
  float gamepad_right_stick_x = 0;
  float gamepad_right_stick_y = 0;
  float gamepad_left_trigger = 0;
  float gamepad_right_trigger = 0;

  std::map<size_t, bool> joystick_inputs;

  float joystick_left_stick_x = 0;
  float joystick_left_stick_y = 0;
  float joystick_right_stick_x = 0;
  float joystick_right_stick_y = 0;
  float joystick_left_trigger = 0;
  float joystick_right_trigger = 0;

 private:
  static constexpr int GAMEPAD_ID = 1;
  static constexpr int JOYSTICK_ID = 0;
};

