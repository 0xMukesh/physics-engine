#include "common/constants.hpp"
#include "core/entity.hpp"
#include "core/scene.hpp"
#include <raylib.h>
#include <vector>

int main() {
  Vector2 bounds = Vector2{SCREEN_WIDTH, SCREEN_HEIGHT};
  InitWindow(bounds.x, bounds.y, "physics engine");
  SetTargetFPS(60);

  Entity ball1 = Entity(30.0, Vector2{SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0},
                        Vector2{0.0, 5.0}, Vector2{0.0, 0.98}, bounds, BLUE);
  Entity ball2 =
      Entity(30.0, Vector2{SCREEN_WIDTH / 2.0 + 50, SCREEN_HEIGHT / 2.0},
             Vector2{0.0, 5.0}, Vector2{0.0, 0.98}, bounds, BLUE);

  std::vector<Entity> entities = {ball1, ball2};

  Scene scene = Scene(entities);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    scene.render();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}