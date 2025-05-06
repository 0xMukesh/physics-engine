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

  // CircularConstraint constraint = CircularConstraint(
  // Vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), 400.0f);

  // CircularObject ball =
  //     CircularObject(5.0f, Vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f),
  //                    Vec2(0.0f, 980.0f), 20.0f, BLUE);

  LineSegmentObject bottomLeftRamp = LineSegmentObject(
      Vec2(0.0f, SCREEN_HEIGHT - 100.0f), Vec2(100.0f, SCREEN_HEIGHT), WHITE);
  LineSegmentObject bottomRightRamp =
      LineSegmentObject(Vec2(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT),
                        Vec2(SCREEN_WIDTH, SCREEN_HEIGHT - 100.0f), WHITE);

  std::vector<std::shared_ptr<Object>> objects = {
      std::make_shared<LineSegmentObject>(bottomLeftRamp),
      std::make_shared<LineSegmentObject>(bottomRightRamp)};

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
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
