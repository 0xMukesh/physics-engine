#pragma once

#include "core/constraint.hpp"
#include "core/object.hpp"
#include <memory>
#include <vector>

class Simulation {
public:
  Simulation(std::vector<std::shared_ptr<Object>> objects,
             std::shared_ptr<Constraint> constraint);
  void update(float dt);
  void render();
  void handleInput();
  void draw();

private:
  void handleCircleCircleCollision(CircularObject *cobj1,
                                   CircularObject *cobj2);
  void checkForStableCluster();

  std::vector<std::shared_ptr<Object>> objects;
  std::shared_ptr<Constraint> constraint;
};
