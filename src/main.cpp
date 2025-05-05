#include "common/constants.hpp"
#include "core/constraint.hpp"
#include "core/entity.hpp"
#include "core/scene.hpp"
#include <memory>
#include <raylib.h>
#include <vector>

int main() {
  Vector2 bounds = Vector2{SCREEN_WIDTH, SCREEN_HEIGHT};
  InitWindow(bounds.x, bounds.y, "physics engine");
  SetTargetFPS(60);

  std::shared_ptr<CircularConstraint> constraint =
      std::make_shared<CircularConstraint>(
          Vector2{SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0}, 200);
  Entity ball =
      Entity(5, 20, Vector2{SCREEN_WIDTH / 2.0 + 30, SCREEN_HEIGHT / 2.0},
             Vector2{0, ACCELERATION_DUE_TO_GRAVITY}, constraint, BLUE);

  Scene scene = Scene({ball}, bounds);

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    scene.update();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawCircleLines(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, 200, YELLOW);

    scene.render(deltaTime);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}