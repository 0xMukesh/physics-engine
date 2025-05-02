#pragma once

#include <raylib.h>

class Entity {
public:
  float radius;
  Vector2 position;
  Vector2 velocity;
  Vector2 acceleration;
  Vector2 bounds;
  Color color;

  Entity(float radius, Vector2 position, Vector2 velocity, Vector2 acceleration,
         Vector2 bounds, Color color);
  void updatePosition();
  void updateVelocity();
  void handleBoundaryCollision();
  void draw();
  void render();
};
