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

  float constraintRadius = 400;

  std::shared_ptr<CircularConstraint> constraint =
      std::make_shared<CircularConstraint>(
          Vector2{SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0}, constraintRadius);
  Entity ball1 = Entity(
      5, 20,
      Vector2{(float)(SCREEN_WIDTH / 2.0), (float)(SCREEN_HEIGHT / 2.0 - 50)},
      Vector2{0, ACCELERATION_DUE_TO_GRAVITY}, constraint, BLUE);
  Entity ball2 =
      Entity(10, 20,
             Vector2{(float)(SCREEN_WIDTH / 2.0), (float)(SCREEN_HEIGHT / 2.0)},
             Vector2{0, ACCELERATION_DUE_TO_GRAVITY}, constraint, BLUE);
  Entity ball3 =
      Entity(10, 20,
             Vector2{(float)(SCREEN_WIDTH / 2.0 + constraintRadius),
                     (float)(SCREEN_HEIGHT / 2.0)},
             Vector2{0, ACCELERATION_DUE_TO_GRAVITY}, constraint, BLUE);

  Scene scene = Scene({ball1, ball2, ball3}, bounds);

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();
    scene.update();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawCircleLines(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, constraintRadius,
                    YELLOW);

    scene.render(deltaTime);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}