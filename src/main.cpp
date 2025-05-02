#include "constants.hpp"
#include "core/entity.hpp"
#include <raylib.h>

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "physics engine");
  SetTargetFPS(60);

  Entity ball = Entity(6.0, Vector2{SCREEN_HEIGHT / 2.0, SCREEN_WIDTH / 2.0},
                       Vector2{0.0, 0.0}, BLUE);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    ball.render();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}