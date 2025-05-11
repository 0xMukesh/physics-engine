#include "simulation.hpp"
#include "common/math.hpp"
#include "core/constraint.hpp"
#include "core/object.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>

Simulation::Simulation(std::vector<std::shared_ptr<Object>> objects,
                       std::shared_ptr<Constraint> constraint)
    : objects(objects), constraint(constraint),
      mouseDragState(
          MouseDragState{.mouseDragStart = nullptr, .isDragging = false}) {}

void Simulation::update(float dt) {
  for (auto object : objects) {
    {
      object->update(dt);
      object->handleConstraint(constraint);
    }
  }

  // handle circle and line collisions
  for (int iter = 0; iter < 2; iter++) {
    for (size_t i = 0; i < objects.size(); i++) {
      for (size_t j = 0; j < objects.size(); j++) {
        if (i == j) {
          continue;
        }

        auto obj1 = objects[i];
        auto obj2 = objects[j];

        auto ciobj1 = dynamic_cast<CircularObject *>(obj1.get());
        auto lobj1 = dynamic_cast<LineSegmentObject *>(obj1.get());
        auto chobj1 = dynamic_cast<ChainObject *>(obj1.get());

        auto ciobj2 = dynamic_cast<CircularObject *>(obj2.get());
        auto lobj2 = dynamic_cast<LineSegmentObject *>(obj2.get());
        auto chobj2 = dynamic_cast<ChainObject *>(obj2.get());

        // circle-line collision
        if (ciobj1 != nullptr && lobj2 != nullptr) {
          handleCircleLineCollision(ciobj1, lobj2);
        }
        if (ciobj2 != nullptr && lobj1 != nullptr) {
          handleCircleLineCollision(ciobj2, lobj1);
        }

        // circle-circle collision
        if (ciobj1 != nullptr && ciobj2 != nullptr) {
          handleCircleCircleCollision(ciobj1, ciobj2);
        }

        // chain-circle collision
        if (chobj1 != nullptr && ciobj2 != nullptr) {
          handleCircleCircleCollision(&chobj1->obj1, ciobj2);
          handleCircleCircleCollision(&chobj1->obj2, ciobj2);
        }
        if (chobj2 != nullptr && ciobj1 != nullptr) {
          handleCircleCircleCollision(&chobj2->obj1, ciobj1);
          handleCircleCircleCollision(&chobj2->obj2, ciobj1);
        }

        // chain-line collision
        if (chobj1 != nullptr && lobj2 != nullptr) {
          handleCircleLineCollision(&chobj1->obj1, lobj2);
          handleCircleLineCollision(&chobj1->obj2, lobj2);
        }
        if (chobj2 != nullptr && lobj1 != nullptr) {
          handleCircleLineCollision(&chobj2->obj1, lobj1);
          handleCircleLineCollision(&chobj2->obj2, lobj1);
        }

        // chain-chain collision
        if (chobj1 != nullptr && chobj2 != nullptr) {
          handleCircleCircleCollision(&chobj1->obj1, &chobj2->obj1);
          handleCircleCircleCollision(&chobj1->obj1, &chobj2->obj2);
          handleCircleCircleCollision(&chobj2->obj1, &chobj1->obj1);
          handleCircleCircleCollision(&chobj2->obj1, &chobj1->obj2);
        }
      }
    }
  }
}

void Simulation::handleCircleCircleCollision(CircularObject *cobj1,
                                             CircularObject *cobj2) {
  float minDist = cobj1->radius + cobj2->radius;
  Vec2 distVec = cobj1->currentPosition - cobj2->currentPosition;
  float distSquared = distVec.length() * distVec.length();

  if (distSquared >= minDist * minDist) {
    return;
  }

  float dist = std::sqrt(distSquared);

  if (dist < 0.001f) {
    float angle = static_cast<float>(GetRandomValue(0, 360)) * DEG2RAD;
    distVec = Vec2(std::cos(angle), std::sin(angle));
    dist = 0.001f;
  } else {
    distVec = distVec / dist;
  }

  float overlap = minDist - dist;

  if (overlap > 0) {
    float totalMass = cobj1->mass + cobj2->mass;
    float ratio1 = cobj2->mass / totalMass;
    float ratio2 = cobj1->mass / totalMass;

    cobj1->currentPosition =
        cobj1->currentPosition + (distVec * overlap * ratio1 * 0.8f);
    cobj2->currentPosition =
        cobj2->currentPosition - (distVec * overlap * ratio2 * 0.8f);
  }
}

void Simulation::handleCircleLineCollision(CircularObject *c,
                                           LineSegmentObject *l) {
  Vec2 lineVec = l->end - l->start;
  Vec2 circleToLineStart = c->currentPosition - l->start;

  float t = circleToLineStart.dot(lineVec) / lineVec.dot(lineVec);
  t = std::max(0.0f, std::min(1.0f, t));

  Vec2 closestPoint = l->start + (lineVec * t);
  Vec2 penetrationVec = c->currentPosition - closestPoint;

  float distance = penetrationVec.length();

  if (distance <= c->radius) {
    if (distance < 0.001f) {
      penetrationVec = Vec2(-lineVec.y, lineVec.x);
      distance = 0.001f;
    } else {
      penetrationVec = penetrationVec / distance;
    }

    float penetrationDepth = c->radius - distance;
    c->currentPosition =
        c->currentPosition + (penetrationVec * penetrationDepth);
  }
}

void Simulation::render() {
  for (auto object : objects) {
    object->render();
  }
}

void Simulation::handleInput() {
  Vector2 mousePosition = GetMousePosition();

  // ctrl + left click = drawing lines
  // alt + left click = adding balls
  // super + left click = chain object
  // shift + hover = deleting lines

  if (IsKeyDown(KEY_LEFT_CONTROL)) {
    if (!mouseDragState.isDragging && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      mouseDragState.isDragging = true;

      if (mouseDragState.mouseDragStart == nullptr) {
        mouseDragState.mouseDragStart =
            new Vector2{mousePosition.x, mousePosition.y};
      } else {
        mouseDragState.mouseDragStart->x = mousePosition.x;
        mouseDragState.mouseDragStart->y = mousePosition.y;
      }
    }

    if (mouseDragState.mouseDragStart) {
      DrawLine(mouseDragState.mouseDragStart->x,
               mouseDragState.mouseDragStart->y, mousePosition.x,
               mousePosition.y, WHITE);
    }
  }

  if ((mouseDragState.isDragging && IsKeyReleased(KEY_LEFT_CONTROL)) ||
      (mouseDragState.isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))) {
    if (mouseDragState.mouseDragStart != nullptr) {
      LineSegmentObject line =
          LineSegmentObject(Vec2(mouseDragState.mouseDragStart->x,
                                 mouseDragState.mouseDragStart->y),
                            Vec2(mousePosition.x, mousePosition.y), WHITE);

      objects.push_back(std::make_shared<LineSegmentObject>(line));
      mouseDragState.isDragging = false;
    }
  }

  if (IsKeyReleased(KEY_LEFT_CONTROL)) {
    mouseDragState.isDragging = false;
    mouseDragState.mouseDragStart = nullptr;
  }

  if (IsKeyDown(KEY_LEFT_ALT) && !IsKeyDown(KEY_SPACE) &&
      IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    float mass = GetRandomValue(10, 50);
    float radius = mass * 0.75;
    Color color =
        Color{.r = static_cast<unsigned char>(GetRandomValue(20, 255)),
              .g = static_cast<unsigned char>(GetRandomValue(20, 255)),
              .b = static_cast<unsigned char>(GetRandomValue(20, 255)),
              .a = static_cast<unsigned char>(GetRandomValue(100, 255))};

    CircularObject ball = CircularObject(mass, Vector2ToVec2(mousePosition),
                                         Vec2(0.0f, 980.0f), radius, color);

    objects.push_back(std::make_shared<CircularObject>(ball));
  }

  if (IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_SPACE) &&
      IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    CircularObject obj1 = CircularObject(10.0f, Vector2ToVec2(mousePosition),
                                         Vec2(0.0f, 980.0f), 10.0f, BLUE);
    CircularObject obj2 =
        CircularObject(10.0f, Vec2(mousePosition.x + 50.0f, mousePosition.y),
                       Vec2(0.0f, 980.0f), 10.0f, BLUE);
    ChainObject chainObj = ChainObject(obj1, obj2, 50.0f);

    objects.push_back(std::make_shared<ChainObject>(chainObj));
  }

  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    Vector2 mousePosition = GetMousePosition();

    for (size_t i = 0; i < objects.size(); i++) {
      auto obj = objects[i];
      auto *object = dynamic_cast<LineSegmentObject *>(obj.get());

      if (object != nullptr) {
        bool isColliding =
            CheckCollisionPointLine(mousePosition, object->start.ToVector2(),
                                    object->end.ToVector2(), 3);

        if (isColliding) {
          objects.erase(objects.begin() + i);
        }
      }
    }
  }
}

void Simulation::draw() {
  auto rc = dynamic_cast<RectangularConstraint *>(constraint.get());
  auto cc = dynamic_cast<CircularConstraint *>(constraint.get());

  if (rc != nullptr) {
    DrawLine(0, 0, rc->width, 0, YELLOW);
    DrawLine(0, 0, 0, rc->height, YELLOW);
    DrawLine(0, rc->height, rc->width, rc->height, YELLOW);
    DrawLine(rc->width, 0, rc->width, rc->height, YELLOW);
  }

  if (cc != nullptr) {
    DrawCircleLinesV(cc->center.ToVector2(), cc->radius, YELLOW);
  }
}
