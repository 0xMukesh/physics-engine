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

void Simulation::handleCircleCircleCollision(CircularObject *cobj1,
                                             CircularObject *cobj2) {
  float minDist = cobj1->radius + cobj2->radius;
  Vec2 distVec = cobj1->currentPosition - cobj2->currentPosition;
  float dist = distVec.length();

  if (dist <= minDist) {
    Vec2 n = distVec / dist;
    float totalMass = cobj1->mass + cobj2->mass;
    float overlap = minDist - dist;

    if (overlap > 0.01f) {
      cobj1->currentPosition =
          cobj1->currentPosition + (n * overlap * (cobj2->mass / totalMass));
      cobj2->currentPosition =
          cobj2->currentPosition - (n * overlap * (cobj1->mass / totalMass));
    }
  }
}

void Simulation::render() {
  for (auto object : objects) {
    object->render();
  }
}

void Simulation::handleInput() {
  Vector2 mousePosition = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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

void Simulation::draw() {}
