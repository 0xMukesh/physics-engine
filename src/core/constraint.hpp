#pragma once

#include "common/math.hpp"

class Constraint {
public:
  Constraint();
  virtual ~Constraint();
};

class CircularConstraint : public Constraint {
public:
  Vec2 center;
  float radius;

  CircularConstraint(Vec2 center, float radius);
};

class RectangularConstraint : public Constraint {
public:
  float width;
  float height;

  RectangularConstraint(float width, float height);
};
