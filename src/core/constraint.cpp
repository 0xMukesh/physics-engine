#include "constraint.hpp"

RectangularConstraint::RectangularConstraint(float width, float height)
    : width(width), height(height) {}

CircularConstraint::CircularConstraint(Vector2 center, float radius)
    : center(center), radius(radius) {}
