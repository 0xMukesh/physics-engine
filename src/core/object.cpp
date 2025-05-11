#include "object.hpp"
#include "common/math.hpp"
#include "core/constraint.hpp"
#include "core/simulation.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

Object::Object() {}
Object::~Object() {}

// CircularObject -- START
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
      oldPosition.y = currentPosition.y + velocity.y * 0.7f;
    }

    if (currentPosition.y - radius < 0) {
      currentPosition.y = radius;
      oldPosition.y = currentPosition.y + velocity.y * 0.7f;
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
// CircularObject -- END

// LineSegmentObject -- START
LineSegmentObject::LineSegmentObject(Vec2 start, Vec2 end, Color color)
    : start(start), end(end), color(color) {}

void LineSegmentObject::update(float _) {}

void LineSegmentObject::render() {
  DrawLine(start.x, start.y, end.x, end.y, color);
}

void LineSegmentObject::handleConstraint(std::shared_ptr<Constraint> _) {}
// LineSegmentObject -- END

// ChainObject -- START
ChainObject::ChainObject(CircularObject obj1, CircularObject obj2,
                         float distance)
    : obj1(obj1), obj2(obj2), distance(distance) {}

void ChainObject::render() {
  obj1.render();
  obj2.render();
  DrawLine(obj1.currentPosition.x, obj1.currentPosition.y,
           obj2.currentPosition.x, obj2.currentPosition.y, WHITE);
}

void ChainObject::update(float dt) {
  obj1.update(dt);
  obj2.update(dt);

  Vec2 direction = obj2.currentPosition - obj1.currentPosition;
  float currentDist = direction.length();

  if (currentDist < 0.0001f) {
    return;
  }

  float diff = (currentDist - distance) / currentDist;
  Vec2 offset = direction * 0.5f * diff;

  obj1.currentPosition = obj1.currentPosition + offset;
  obj2.currentPosition = obj2.currentPosition - offset;
}

void ChainObject::handleConstraint(std::shared_ptr<Constraint> constraint) {
  obj1.handleConstraint(constraint);
  obj2.handleConstraint(constraint);
}
// ChainObject -- END
