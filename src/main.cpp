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
  SetTargetFPS(120);

  RectangularConstraint constraint =
      RectangularConstraint(SCREEN_WIDTH, SCREEN_HEIGHT);

  //   CircularObject obj1 =
  //       CircularObject(10.0f, Vec2(0, 0), Vec2(0, 980.0f), 20.0f, BLUE);
  //   CircularObject obj2 =
  //       CircularObject(10.0f, Vec2(140, 0), Vec2(0, 980.0f), 20.0f, BLUE);
  //   ChainObject chainObj = ChainObject(obj1, obj2, 100.0f);

  //   LineSegmentObject lineSegmentObj =
  //       LineSegmentObject(Vec2(0, 50), Vec2(1000, 600), WHITE);

  std::vector<std::shared_ptr<Object>> objects = {};

  Simulation simulation =
      Simulation(objects, std::make_shared<RectangularConstraint>(constraint));

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    simulation.update(dt);
    simulation.handleInput();

    BeginDrawing();
    ClearBackground(BLACK);
    simulation.draw();
    simulation.render();
    DrawFPS(10, 10);
    DrawText(TextFormat("Objects: %d", simulation.objects.size()), 10, 40, 20,
             LIME);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
