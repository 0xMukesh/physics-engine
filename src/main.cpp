#include "common/constants.hpp"
#include "common/math.hpp"
#include "core/constraint.hpp"
#include "core/object.hpp"
#include "core/simulation.hpp"
#include <memory>
#include <raylib.h>
#include <vector>

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "simulation");
  SetTargetFPS(60);

  RectangularConstraint constraint =
      RectangularConstraint(SCREEN_WIDTH, SCREEN_HEIGHT);

  CircularObject ball =
      CircularObject(5.0f, Vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f),
                     Vec2(0.0f, 980.0f), 20.0f, BLUE);

  std::vector<std::shared_ptr<Object>> objects = {
      std::make_shared<CircularObject>(ball)};

  Simulation simulation =
      Simulation(objects, std::make_shared<RectangularConstraint>(constraint));

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    simulation.update(dt);
    simulation.handleInput();

    BeginDrawing();
    ClearBackground(BLACK);
    simulation.render();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
