#pragma once

#include <raylib.h>

class Entity {
public:
  float radius;
  Vector2 position;
  Vector2 velocity;
  Vector2 acceleration;
  Color color;

  Entity(float radius, Vector2 position, Vector2 acceleration, Color color);
  void updatePosition();
  void updateVelocity();
  void draw();
  void render();
};
