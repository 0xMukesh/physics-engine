#pragma once

#include "common/math.hpp"
#include "core/constraint.hpp"
#include "raylib.h"
#include <memory>

class Object {
public:
  Object();
  virtual ~Object();

  virtual void update(float dt) = 0;
  virtual void render() = 0;
  virtual void handleConstraint(std::shared_ptr<Constraint> constraint) = 0;
};

class CircularObject : public Object {
public:
  float mass;
  float radius;
  Vec2 currentPosition;
  Vec2 oldPosition;
  Vec2 acceleration;
  Color color;

  CircularObject(float mass, Vec2 position, Vec2 acceleration, float radius,
                 Color color);

  void update(float dt) override;
  void render() override;
  void handleConstraint(std::shared_ptr<Constraint> constraint) override;
};

class LineSegmentObject : public Object {
public:
  Vec2 start;
  Vec2 end;
  Color color;

  LineSegmentObject(Vec2 start, Vec2 end, Color color);

  void update(float dt) override;
  void render() override;
  void handleConstraint(std::shared_ptr<Constraint> constraint) override;
};
