#pragma once

#include <raylib.h>

class Entity {
public:
  float mass;
  float radius;
  Vector2 position;
  Vector2 velocity;
  Vector2 acceleration;
  Vector2 bounds;
  Color color;
  bool isSelected;

  Entity(float mass, float radius, Vector2 position, Vector2 velocity,
         Vector2 acceleration, Vector2 bounds, Color color);
  void updatePosition(float deltaTime);
  void updateVelocity(float deltaTime);
  void handleBoundaryCollision();
  void handleEntityCollision(Entity &entity);
  void draw();
  void render(float deltaTime);
};
