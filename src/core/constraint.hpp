#pragma once

#include <raylib.h>

class Constraint {
public:
  virtual ~Constraint() = default;
};

class RectangularConstraint : public Constraint {
public:
  float width;
  float height;

  RectangularConstraint(float width, float height);
};

class CircularConstraint : public Constraint {
public:
  Vector2 center;
  float radius;

  CircularConstraint(Vector2 center, float radius);
};
