#include "common/constants.hpp"
#include "common/math.hpp"
#include "core/constraint.hpp"
#include "core/object.hpp"
#include "core/simulation.hpp"
#include <cmath>
#include <memory>
#include <raylib.h>
#include <vector>

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "simulation");
  SetTargetFPS(120);

  RectangularConstraint constraint =
      RectangularConstraint(SCREEN_WIDTH, SCREEN_HEIGHT);

  std::vector<std::shared_ptr<Object>> objects = {};
  Simulation simulation(SCREEN_WIDTH, SCREEN_HEIGHT, objects,
                        std::make_shared<RectangularConstraint>(constraint),
                        true, 10.0f);

  Vec2 startPosition(SCREEN_WIDTH / 2.0f, 50);
  Vec2 gravity(0, 980.0f);

  float spawnTimer = 0.0f;
  float elapsedTime = 0.0f;
  float spawnInterval = 0.1f;
  int particleIndex = 0;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    spawnTimer += dt;
    elapsedTime += dt;

    while (spawnTimer >= spawnInterval) {
      spawnTimer -= spawnInterval;

      Color color = particleIndex % 2 == 0   ? RED
                    : particleIndex % 3 == 0 ? BLUE
                                             : GREEN;

      CircularObject obj(5.0f, startPosition, gravity, 5.0f, color);
      float angle = -PI / 2.0f * std::cosf(elapsedTime * 2.0f);

      Vec2 direction = Vec2(std::sinf(angle), std::cosf(angle));
      obj.oldPosition = obj.currentPosition - direction * 200.0f * dt;

      if (simulation.objects.size() >= 500) {
        break;
      }

      simulation.objects.push_back(std::make_shared<CircularObject>(obj));
      particleIndex++;
    }

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
