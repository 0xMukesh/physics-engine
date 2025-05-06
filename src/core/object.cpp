#include "object.hpp"
#include "common/math.hpp"
#include "core/constraint.hpp"

Object::Object() {}
Object::~Object() {}

CircularObject::CircularObject(float mass, Vec2 position, Vec2 acceleration,
                               float radius, Color color)
    : mass(mass), currentPosition(position), oldPosition(position),
      acceleration(acceleration), radius(radius), color(color) {}

void CircularObject::render() {
  DrawCircle(currentPosition.x, currentPosition.y, radius, color);
}

void CircularObject::update(float dt) {
  Vec2 temp = currentPosition;
  currentPosition =
      (2.0f * currentPosition) - oldPosition + (acceleration * dt * dt);
  oldPosition = temp;
}

void CircularObject::handleConstraint(std::shared_ptr<Constraint> constraint) {
  RectangularConstraint *rc =
      dynamic_cast<RectangularConstraint *>(constraint.get());

  if (rc != nullptr) {
    if (currentPosition.y + radius >= rc->height) {
      currentPosition.y = rc->height - radius;
    }

    if (currentPosition.y - radius <= 0) {
      currentPosition.y = radius;
    }

    if (currentPosition.x + radius >= rc->width) {
      currentPosition.x = rc->width - radius;
    }

    if (currentPosition.x - radius <= 0) {
      currentPosition.x = radius;
    }
  }
}
