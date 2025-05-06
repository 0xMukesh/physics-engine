#include "object.hpp"
#include "common/math.hpp"
#include "core/constraint.hpp"

Object::Object() {}
Object::~Object() {}

CircularObject::CircularObject(float mass, Vec2 position, Vec2 acceleration,
                               float radius, Color color)
    : mass(mass), radius(radius), currentPosition(position),
      oldPosition(position), acceleration(acceleration), color(color) {}

void CircularObject::render() {
  DrawCircle(currentPosition.x, currentPosition.y, radius, color);
}

void CircularObject::update(float dt) {
  Vec2 temp = currentPosition;
  Vec2 velocity = (currentPosition - oldPosition) * 0.998f;
  currentPosition = currentPosition + velocity + (acceleration * dt * dt);
  oldPosition = temp;
}
void CircularObject::handleConstraint(std::shared_ptr<Constraint> constraint) {
  RectangularConstraint *rc =
      dynamic_cast<RectangularConstraint *>(constraint.get());
  CircularConstraint *cc = dynamic_cast<CircularConstraint *>(constraint.get());

  if (rc != nullptr) {
    Vec2 velocity = currentPosition - oldPosition;

    if (currentPosition.y + radius > rc->height) {
      currentPosition.y = rc->height - radius;
    }

    if (currentPosition.y - radius < 0) {
      currentPosition.y = radius;
    }

    if (currentPosition.x + radius > rc->width) {
      currentPosition.x = rc->width - radius;
      oldPosition.x = currentPosition.x + velocity.x * 0.7f;
    }

    if (currentPosition.x - radius < 0) {
      currentPosition.x = radius;
      oldPosition.x = currentPosition.x + velocity.x * 0.7f;
    }
  }

  if (cc != nullptr) {
    float minDist = cc->radius - radius;
    Vec2 distVec = currentPosition - cc->center;
    float dist = distVec.length();

    if (dist > minDist && dist > 0.001f) {
      Vec2 n = distVec / dist;
      currentPosition = cc->center + n * minDist;
    }
  }
}
