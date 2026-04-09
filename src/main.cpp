#include "control.h"
#include "raylib.h"

int main() {
  GamepadControlProxy controller_info;
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "EvilAwesomeBagleSimulator");

  Camera3D camera = {0};
  camera.position = (Vector3){10.0f, 10.0f, 10.0f};  // Camera position
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};       // Camera looking at point
  camera.up = (Vector3){0.0f, 1.0f,
                        0.0f};  // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;          // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE;  // Camera projection type

  DisableCursor();

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    controller_info.step();

    UpdateCamera(&camera, CAMERA_FREE);

    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    DrawGrid(10, 1.0f);

    EndMode3D();

    // DrawText("Congrats! You created your first window!", 190, 200, 20,
    //          LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
