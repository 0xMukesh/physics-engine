#pragma once

#include "core/constraint.hpp"
#include "core/object.hpp"
#include <memory>
#include <vector>

class Simulation {
public:
  std::vector<std::shared_ptr<Object>> objects;
  std::shared_ptr<Constraint> constraint;

  Simulation(std::vector<std::shared_ptr<Object>> objects,
             std::shared_ptr<Constraint> constraint);

  void update(float dt);
  void handleInput();
  void render();
  void draw();

private:
  void handleCircleCircleCollision(CircularObject *c1, CircularObject *c2);
};
