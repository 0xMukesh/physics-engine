#pragma once

#include "core/constraint.hpp"
#include "core/object.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

struct MouseDragState {
  Vector2 *mouseDragStart;
  bool isDragging;
};

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
  void handleCircleLineCollision(CircularObject *c, LineSegmentObject *l);

  std::vector<std::shared_ptr<Object>> objects;
  std::shared_ptr<Constraint> constraint;
  MouseDragState mouseDragState;
};
