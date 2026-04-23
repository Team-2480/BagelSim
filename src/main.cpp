#include "config.h"
#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "scene.h"

class MenuScene final : public Scene {
 private:
  struct nk_context* ctx;
  Font font;
  Model map_model;
  Shader& shader;
  Camera ui_camera{
      .position = Vector3{0.0f, -5.0f, 10.0f},
      .target = Vector3{0.0f, 0.0f, 0.0f},
      .up = Vector3{0.0f, 1.0f, 0.0f},
      .fovy = 100.0f,
      .projection = CAMERA_PERSPECTIVE,
  };
  Camera camera{
      .position = Vector3{0.0f, 4.0f, 10.0f},
      .target = Vector3{0.0f, 0.0f, 0.0f},
      .up = Vector3{0.0f, 1.0f, 0.0f},
      .fovy = 100.0f,
      .projection = CAMERA_PERSPECTIVE,
  };

  Mesh torus = GenMeshTorus(0.4f, 6, 10, 8);
  Model torus_model = LoadModelFromMesh(torus);
  Model name_model = LoadModel(RELEASE_FOLDER("name.glb"));
  float torus_rotate;

 public:
  MenuScene(ProgramState& program_state, Shader& shader)
      : Scene(program_state), shader(shader) {
    int font_size = 20;
    font = LoadFontEx((Constants::release_folder + "Lato-Regular.ttf").c_str(),
                      20, NULL, 0);
    ctx = InitNuklearEx(font, font_size);

    map_model = LoadModel(RELEASE_FOLDER("map.glb"));
    for (int i = 0; i < map_model.materialCount; i++) {
      map_model.materials[i].shader = shader;
    }
  }
  ~MenuScene() {
    UnloadNuklear(ctx);
    UnloadFont(font);
    UnloadModel(map_model);
    UnloadModel(torus_model);
  }

  void draw() override {
    float cameraPos[3] = {camera.position.x, camera.position.y,
                          camera.position.z};
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos,
                   SHADER_UNIFORM_VEC3);

    BeginMode3D(camera);

    BeginShaderMode(shader);

    DrawModel(map_model, Vector3Zero(), 1, WHITE);

    EndShaderMode();
    EndMode3D();

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});

    BeginMode3D(ui_camera);
    // BeginShaderMode(shader);

    rlPushMatrix();
    rlTranslatef(0, 3, 0);
    rlRotatef(-30, 1, 0, 0);
    rlRotatef(torus_rotate, 0, 0, 1);
    torus_rotate += 1;

    DrawModel(torus_model, {0, 0, 0}, 1, BEIGE);
    rlPopMatrix();

    rlPushMatrix();
    rlTranslatef(0, 3, 0);
    rlRotatef(-30, 1, 0, 0);

    DrawModel(name_model, {0, 10, 5}, 1, WHITE);
    rlPopMatrix();

    // EndShaderMode();
    EndMode3D();

    DrawNuklear(ctx);
  }
  void step() override {
    UpdateNuklear(ctx);

    /*
        uint32_t padding = 10;
        auto center_x = GetScreenWidth() / 2;
        auto width_x = GetScreenWidth() - padding * 2;
        auto center_y = GetScreenHeight() / 2;
        auto width_y = GetScreenHeight() - padding * 2;
    */

    float center_x = GetScreenWidth() / 2.0f;
    float width_x = std::min(600, GetScreenWidth());
    float center_y = GetScreenHeight() / 2.0f;
    float width_y = std::min(300, GetScreenHeight());

    ctx->style.window.fixed_background = nk_style_item_color({0, 0, 0, 0});
    if (nk_begin(ctx, "Nuklear",
                 nk_rect(center_x - width_x / 2, center_y - width_y / 2,
                         width_x, width_y),
                 NK_WINDOW_BACKGROUND)) {
      nk_layout_row_dynamic(ctx, 50, 1);

      // logo
      nk_spacer(ctx);
      nk_spacer(ctx);
      nk_spacer(ctx);

      if (nk_button_label(ctx, "Play")) {
        state.screen = ProgramState::SCREEN_GAME;
      }
    }
    nk_end(ctx);

    CameraYaw(&camera, -0.1 * DEG2RAD, true);
  }
};

static Light lights[MAX_LIGHTS];
static Shader shader;

class SceneManager {
 private:
  ProgramState state;

 public:
  SceneManager() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitWindow(screenWidth, screenHeight, "BagelSim");

    shader = LoadShader((Constants::release_folder + "lighting.vs").c_str(),
                        (Constants::release_folder + "lighting.fs").c_str());
#ifdef PLATFORM_WEB
    SetTraceLogLevel(LOG_ERROR);
#endif

    SetTargetFPS(30);
    /*
    is the max fps locked at 30 for you too? (doesn't reach 60
    even when SetTargetFPS(60))
    */

    JoltWrapper::init();

    game_scene = new GameScene(state, shader);
    menu_scene = new MenuScene(state, shader);
  }
  ~SceneManager() {
    JoltWrapper::free();
    if (game_scene.has_value()) {
      delete game_scene.value();
    }
    if (menu_scene.has_value()) {
      delete menu_scene.value();
    }
    UnloadShader(shader);
  }
  std::optional<GameScene*> game_scene = std::nullopt;
  std::optional<MenuScene*> menu_scene = std::nullopt;
  std::optional<Scene*> scene = std::nullopt;

  void step() {
    switch (state.screen) {
      case ProgramState::SCREEN_MAIN_MENU:
        scene = static_cast<Scene*>(menu_scene.value());
        break;
      case ProgramState::SCREEN_GAME:
        scene = static_cast<Scene*>(game_scene.value());
        break;
    }
    if (scene.has_value()) {
      scene.value()->step();
    }
    BeginDrawing();

    ClearBackground(BLACK);
    if (scene.has_value()) {
      scene.value()->draw();
    }

    EndDrawing();
  }

 private:
  const int screenWidth = 800;
  const int screenHeight = 450;
};

static SceneManager* manager;

void step() { manager->step(); }

int main() {
  manager = new SceneManager;

  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

  int ambient_loc = GetShaderLocation(shader, "ambient");
  float ambient_lighting[4] = {0.1f, 0.1f, 0.1f, 1.0f};
  SetShaderValue(shader, ambient_loc, ambient_lighting, SHADER_UNIFORM_VEC4);

  lights[0] = CreateLight(LIGHT_POINT, Vector3{0, 4, -4}, Vector3Zero(),
                          Color{50, 50, 50, 50}, shader, 0);
  lights[1] = CreateLight(LIGHT_POINT, Vector3{0, 4, 4}, Vector3Zero(),
                          Color{50, 50, 50, 50}, shader, 1);
  lights[2] = CreateLight(LIGHT_POINT, Vector3{-10, 4, 0}, Vector3Zero(),
                          Color{50, 50, 50, 50}, shader, 2);
  lights[3] = CreateLight(LIGHT_POINT, Vector3{10, 4, 0}, Vector3Zero(),
                          Color{50, 50, 50, 50}, shader, 3);

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(step, 0, 1);
#else
  while (!WindowShouldClose()) {
    step();
  }
#endif

  CloseWindow();

  delete manager;

  return 0;
}
