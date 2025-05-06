#include "constraint.hpp"

Constraint::Constraint() {}
Constraint::~Constraint() {}

CircularConstraint::CircularConstraint(Vec2 center, float radius)
    : center(center), radius(radius) {}

RectangularConstraint::RectangularConstraint(float width, float height)
    : width(width), height(height) {}