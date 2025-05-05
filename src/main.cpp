#include "common/constants.hpp"
#include "core/scene.hpp"
#include <raylib.h>
#include <vector>

int main() {
  Vector2 bounds = Vector2{SCREEN_WIDTH, SCREEN_HEIGHT};
  InitWindow(bounds.x, bounds.y, "physics engine");
  SetTargetFPS(60);

  Scene scene = Scene({}, bounds);

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    scene.update();

    BeginDrawing();
    ClearBackground(BLACK);
    scene.render(deltaTime);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}