#include "constants.hpp"
#include "core/entity.hpp"
#include <raylib.h>

int main() {
  Vector2 bounds = Vector2{SCREEN_WIDTH, SCREEN_HEIGHT};
  InitWindow(bounds.x, bounds.y, "physics engine");
  SetTargetFPS(60);

  Entity ball = Entity(6.0, Vector2{SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0},
                       Vector2{0.0, 5.0}, Vector2{0.0, 0.0}, bounds, BLUE);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    ball.render();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}