#include "simulation.hpp"
#include "common/math.hpp"
#include "core/constraint.hpp"
#include "core/object.hpp"
#include "raylib.h"
#include <cmath>
#include <cstddef>
#include <memory>

Simulation::Simulation(std::vector<std::shared_ptr<Object>> objects,
                       std::shared_ptr<Constraint> constraint)
    : objects(objects), constraint(constraint) {}

void Simulation::update(float dt) {
  for (auto object : objects) {
    {
      object->update(dt);
      object->handleConstraint(constraint);
    }
  }

  for (int iter = 0; iter < 2; iter++) {
    for (size_t i = 0; i < objects.size(); i++) {
      for (size_t j = 0; j < objects.size(); j++) {
        if (i == j) {
          continue;
        }

        auto obj1 = objects[i];
        auto obj2 = objects[j];

        auto cobj1 = dynamic_cast<CircularObject *>(obj1.get());
        auto cobj2 = dynamic_cast<CircularObject *>(obj2.get());

        if (cobj1 != nullptr && cobj2 != nullptr) {
          handleCircleCircleCollision(cobj1, cobj2);
        }
      }
    }
  }
}

void Simulation::handleCircleCircleCollision(CircularObject *cobj1,
                                             CircularObject *cobj2) {
  float minDist = cobj1->radius + cobj2->radius;
  Vec2 distVec = cobj1->currentPosition - cobj2->currentPosition;
  float distSquared = distVec.length() * distVec.length();

  if (distSquared >= minDist * minDist) {
    return;
  }

  float dist = std::sqrt(distSquared);

  if (dist < 0.001f) {
    float angle = static_cast<float>(GetRandomValue(0, 360)) * DEG2RAD;
    distVec = Vec2(std::cos(angle), std::sin(angle));
    dist = 0.001f;
  } else {
    distVec = distVec / dist;
  }

  float overlap = minDist - dist;

  if (overlap > 0) {
    float totalMass = cobj1->mass + cobj2->mass;
    float ratio1 = cobj2->mass / totalMass;
    float ratio2 = cobj1->mass / totalMass;

    cobj1->currentPosition =
        cobj1->currentPosition + (distVec * overlap * ratio1 * 0.8f);
    cobj2->currentPosition =
        cobj2->currentPosition - (distVec * overlap * ratio2 * 0.8f);
  }
}

void Simulation::render() {
  for (auto object : objects) {
    object->render();
  }
}

void Simulation::handleInput() {
  Vector2 mousePosition = GetMousePosition();

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    float mass = GetRandomValue(10, 50);
    float radius = mass * 0.75;
    Color color =
        Color{.r = static_cast<unsigned char>(GetRandomValue(20, 255)),
              .g = static_cast<unsigned char>(GetRandomValue(20, 255)),
              .b = static_cast<unsigned char>(GetRandomValue(20, 255)),
              .a = static_cast<unsigned char>(GetRandomValue(100, 255))};

    CircularObject ball = CircularObject(mass, Vector2ToVec2(mousePosition),
                                         Vec2(0.0f, 980.0f), radius, color);

    objects.push_back(std::make_shared<CircularObject>(ball));
  }
}

void Simulation::draw() {
  auto rc = dynamic_cast<RectangularConstraint *>(constraint.get());
  auto cc = dynamic_cast<CircularConstraint *>(constraint.get());

  if (rc != nullptr) {
    DrawLine(0, 0, rc->width, 0, YELLOW);
    DrawLine(0, 0, 0, rc->height, YELLOW);
    DrawLine(0, rc->height, rc->width, rc->height, YELLOW);
    DrawLine(rc->width, 0, rc->width, rc->height, YELLOW);
  }

  if (cc != nullptr) {
    DrawCircleLinesV(Vec2ToVector2(cc->center), cc->radius, YELLOW);
  }
}
