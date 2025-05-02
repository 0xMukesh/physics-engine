#include "entity.hpp"
#include "common/constants.hpp"
#include "common/utils.hpp"
#include <raylib.h>

Entity::Entity(float radius, Vector2 position, Vector2 velocity,
               Vector2 acceleration, Vector2 bounds, Color color)
    : radius(radius), position(position), velocity(velocity),
      acceleration(acceleration), bounds(bounds), color(color) {}

void Entity::updatePosition() {
  position.x += velocity.x;
  position.y += velocity.y;
}

void Entity::updateVelocity() {
  velocity.x += acceleration.x;
  velocity.y += acceleration.y;
}

void Entity::handleBoundaryCollision() {
  float screenWidth = bounds.x;
  float screenHeight = bounds.y;

  // left side of the screen
  if (position.x + radius <= 0) {
    position.x = radius;
  }

  // right side of the screen
  if (position.x + radius >= screenWidth) {
    position.x = screenWidth - radius;
  }

  // top side of the screen
  if (position.y + radius <= 0) {
    position.y = radius;
    velocity.y = utils::clipNumber(velocity.y, VELOCITY_REST_THRESHOLD);
    velocity.y *= -0.8;
  }

  // bottom side of the screen
  if (position.y + radius >= screenHeight) {
    position.y = screenHeight - radius;
    velocity.y = utils::clipNumber(velocity.y, VELOCITY_REST_THRESHOLD);
    velocity.y *= -0.8;
  }
}

void Entity::draw() { DrawCircle(position.x, position.y, radius, color); }

void Entity::render() {
  updateVelocity();
  updatePosition();
  handleBoundaryCollision();
  draw();
}
