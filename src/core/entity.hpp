#pragma once

#include "core/constraint.hpp"
#include <memory>
#include <raylib.h>

class Entity {
public:
  float mass;
  float radius;
  Vector2 positionCurrent;
  Vector2 positionOld;
  Vector2 acceleration;
  std::shared_ptr<Constraint> constraint;
  Color color;

  Entity(float mass, float radius, Vector2 position, Vector2 acceleration,
         std::shared_ptr<Constraint> constraint, Color color);
  void render(float deltaTime);
  void handleEntityCollision(Entity &entity);

private:
  void updatePosition(float deltaTime);
  void handleBoundaryCollision();
  void draw();
};