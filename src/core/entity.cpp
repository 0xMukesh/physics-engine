#include "entity.hpp"
#include <raylib.h>

Entity::Entity(float radius, Vector2 position, Vector2 acceleration,
               Color color)
    : radius(radius), position(position), velocity(Vector2{0.0, 0.0}),
      acceleration(acceleration), color(color) {}

void Entity::updatePosition() {
  position.x += velocity.x;
  position.y += velocity.y;
}

void Entity::updateVelocity() {
  velocity.x += acceleration.x;
  velocity.y += acceleration.y;
}

void Entity::draw() { DrawCircle(position.x, position.y, radius, color); }

void Entity::render() {
  updateVelocity();
  updatePosition();
  draw();
}
