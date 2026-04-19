#pragma once

#include <GLFW/glfw3.h>

#include <cstddef>
#include <map>
#include <print>

#include "raylib.h"
class GamepadControlProxy {
 public:
  GamepadControlProxy() {}
  ~GamepadControlProxy() {}

  void step() {
    last_gamepad_inputs = gamepad_inputs;
    last_gamepad_axis = gamepad_axis;
    last_joystick_inputs = joystick_inputs;
    last_joystick_axis = joystick_axis;

    if (keyboard_overide) {
      has_joystick = true;
      joystick_axis[0] = IsKeyDown(KEY_D) + -1 * IsKeyDown(KEY_A);
      joystick_axis[1] = IsKeyDown(KEY_S) + -1 * IsKeyDown(KEY_W);
      joystick_axis[2] = IsKeyDown(KEY_L) + -1 * IsKeyDown(KEY_J);

      joystick_inputs[4] = IsKeyDown(KEY_Q);
      joystick_inputs[11] = IsKeyDown(KEY_E);

    } else {
      has_gamepad = IsGamepadAvailable(GAMEPAD_ID);
      if (IsGamepadAvailable(GAMEPAD_ID)) {
        int count;
        const float* axes = glfwGetJoystickAxes(GAMEPAD_ID, &count);

        for (int i = 0; i < count; i++) {
          joystick_axis[i] = axes[i];
        }

        auto buttons = glfwGetJoystickButtons(GAMEPAD_ID, &count);
        for (size_t i = 0; i < count; i++) {
          gamepad_inputs[i] = buttons[i] == GLFW_PRESS;
        }
      }

      has_joystick = IsGamepadAvailable(JOYSTICK_ID);
      if (IsGamepadAvailable(JOYSTICK_ID)) {
        int count;
        const float* axes = glfwGetJoystickAxes(JOYSTICK_ID, &count);

        for (int i = 0; i < count; i++) {
          joystick_axis[i] = axes[i];
        }

        auto buttons = glfwGetJoystickButtons(JOYSTICK_ID, &count);
        for (size_t i = 0; i < count; i++) {
          joystick_inputs[i] = buttons[i] == GLFW_PRESS;
        }
      }
    }
  }

  bool keyboard_overide = false;

  bool has_gamepad = false;
  std::map<size_t, bool> gamepad_inputs;
  std::map<size_t, float> gamepad_axis;
  std::map<size_t, bool> last_gamepad_inputs;
  std::map<size_t, float> last_gamepad_axis;

  bool has_joystick = false;
  std::map<size_t, bool> joystick_inputs;
  std::map<size_t, float> joystick_axis;
  std::map<size_t, bool> last_joystick_inputs;
  std::map<size_t, float> last_joystick_axis;

 private:
  static constexpr int GAMEPAD_ID = 1;
  static constexpr int JOYSTICK_ID = 0;
};

