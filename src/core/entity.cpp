#include "entity.hpp"
#include <cmath>
#include <raylib.h>

Entity::Entity(float mass, float radius, Vector2 position, Vector2 velocity,
               Vector2 acceleration, Vector2 bounds, Color color)
    : mass(mass), radius(radius), position(position), velocity(velocity),
      acceleration(acceleration), bounds(bounds), color(color),
      isSelected(false) {}

void Entity::updatePosition(float deltaTime) {
  position.x += velocity.x * deltaTime;
  position.y += velocity.y * deltaTime;
}

void Entity::updateVelocity(float deltaTime) {
  velocity.x += acceleration.x * deltaTime;
  velocity.y += acceleration.y * deltaTime;
}

void Entity::handleBoundaryCollision() {
  float screenWidth = bounds.x;
  float screenHeight = bounds.y;

  if (position.x - radius <= 0) {
    position.x = radius;
    velocity.x *= -0.8;
  }

  if (position.x + radius >= screenWidth) {
    position.x = screenWidth - radius;
    velocity.x *= -0.8;
  }

  if (position.y - radius <= 0 && velocity.y < 0) {
    position.y = radius;
    velocity.y *= -0.8;
  }

  if (position.y + radius >= screenHeight) {
    position.y = screenHeight - radius;
    velocity.y *= -0.8;
  }
}

void Entity::handleEntityCollision(Entity &entity) {
  float xDist = position.x - entity.position.x;
  float yDist = position.y - entity.position.y;

  float dist = std::sqrt(xDist * xDist + yDist * yDist);
  float minDist = entity.radius + radius;

  if (dist <= minDist) {
    DrawText("Collision", 10, 10, 30, RED);

    float nx = xDist / dist;
    float ny = yDist / dist;

    float relVelX = velocity.x - entity.velocity.x;
    float relVelY = velocity.y - entity.velocity.y;

    float dotProduct = relVelX * nx + relVelY * ny;

    if (dotProduct > 0)
      return;

    float impulse = (2 * dotProduct) / (mass + entity.mass);

    velocity.x -= (impulse * mass * nx);
    velocity.y -= (impulse * mass * ny);
    entity.velocity.x += (impulse * entity.mass * nx);
    entity.velocity.y += (impulse * entity.mass * ny);

    float overlap = minDist - dist;
    const float percent = 0.8f;
    const float slop = 0.01f;

    if (overlap > slop) {
      float correction = (overlap - slop) / (mass + entity.mass) * percent;
      position.x += correction * mass * nx;
      position.y += correction * mass * ny;
      entity.position.x -= correction * entity.mass * nx;
      entity.position.y -= correction * entity.mass * ny;
    }
  }
}

void Entity::draw() {
  DrawCircle(position.x, position.y, radius, color);

  if (isSelected) {
    DrawCircleLines(position.x, position.y, radius + 2.0f, YELLOW);
  }
}

void Entity::render(float deltaTime) {
  updateVelocity(deltaTime);
  updatePosition(deltaTime);
  handleBoundaryCollision();
  draw();
}
