#include "entity.hpp"
#include "common/constants.hpp"
#include "core/constraint.hpp"
#include <cmath>
#include <raylib.h>

Entity::Entity(float mass, float radius, Vector2 position, Vector2 acceleration,
               std::shared_ptr<Constraint> constraint, Color color)
    : mass(mass), radius(radius), positionCurrent(position),
      positionOld(position), acceleration(acceleration),
      constraint(std::move(constraint)), color(color) {}

void Entity::updatePosition(float deltaTime) {
  Vector2 temp = positionCurrent;

  positionCurrent.x = (2 * positionCurrent.x) - (positionOld.x) +
                      acceleration.x * deltaTime * deltaTime;
  positionCurrent.y = (2 * positionCurrent.y) - (positionOld.y) +
                      acceleration.y * deltaTime * deltaTime;

  positionOld = temp;
}

void Entity::handleBoundaryCollision() {
  if (!constraint) {
    return;
  }

  RectangularConstraint *rectConstraint =
      dynamic_cast<RectangularConstraint *>(constraint.get());
  CircularConstraint *circularConstraint =
      dynamic_cast<CircularConstraint *>(constraint.get());

  if (rectConstraint != nullptr) {
    float width = rectConstraint->width;
    float height = rectConstraint->height;

    float horizontalDampingFactor = 0.75f;
    float verticalDampingFactor = 0.5f;
    float velocityThreshold = 0.1f;

    Vector2 velocity = {positionCurrent.x - positionOld.x,
                        positionCurrent.y - positionOld.y};

    if (positionCurrent.x - radius < 0) {
      positionCurrent.x = radius;
      if (fabs(velocity.x) > velocityThreshold) {
        positionOld.x =
            positionCurrent.x + velocity.x * horizontalDampingFactor;
      } else {
        positionOld.x = positionCurrent.x;
      }
    }

    if (positionCurrent.x + radius > width) {
      positionCurrent.x = width - radius;
      if (fabs(velocity.x) > velocityThreshold) {
        positionOld.x =
            positionCurrent.x + velocity.x * horizontalDampingFactor;
      } else {
        positionOld.x = positionCurrent.x;
      }
    }

    if (positionCurrent.y - radius < 0) {
      positionCurrent.y = radius;
      if (fabs(velocity.y) > velocityThreshold) {
        positionOld.y = positionCurrent.y + velocity.y * verticalDampingFactor;
      } else {
        positionOld.y = positionCurrent.y;
      }
    }

    if (positionCurrent.y + radius > height) {
      positionCurrent.y = height - radius;
      if (fabs(velocity.y) > velocityThreshold) {
        positionOld.y = positionCurrent.y + velocity.y * verticalDampingFactor;
      } else {
        positionOld.y = positionCurrent.y;
      }

      if (fabs(velocity.x) > 0) {
        float rollingFriction = 0.95f;
        positionOld.x = positionCurrent.x + (velocity.x * rollingFriction);

        if (fabs(velocity.x) < 0.1f) {
          positionOld.x = positionCurrent.x;
        }
      }
    }
  } else if (circularConstraint != nullptr) {
    Vector2 constraintCenter = circularConstraint->center;
    float constraintRadius = circularConstraint->radius;

    float xDist = positionCurrent.x - constraintCenter.x;
    float yDist = positionCurrent.y - constraintCenter.y;
    float dist = std::sqrt(xDist * xDist + yDist * yDist);

    if (dist > constraintRadius - radius) {
      float nx = 0.0f, ny = 0.0f;
      if (dist > 0.0001f) {
        nx = xDist / dist;
        ny = yDist / dist;
      } else {
        nx = GetRandomValue(-100, 100) / 100.0f;
        ny = GetRandomValue(-100, 100) / 100.0f;
        float randNormLength = std::sqrt(nx * nx + ny * ny);
        nx /= randNormLength;
        ny /= randNormLength;
      }

      float targetDist = constraintRadius - radius;
      if (targetDist < 0)
        targetDist = 0;

      positionCurrent.x = constraintCenter.x + nx * targetDist;
      positionCurrent.y = constraintCenter.y + ny * targetDist;

      Vector2 vel = Vector2{positionCurrent.x - positionOld.x,
                            positionCurrent.y - positionOld.y};

      // v_new = v - 2 * dot(v, n) * n
      float velDotNormal = vel.x * nx + vel.y * ny;
      float reflectedVelX = (vel.x - 2 * velDotNormal * nx);
      float reflectedVelY = (vel.y - 2 * velDotNormal * ny);

      positionOld.x = positionCurrent.x - reflectedVelX;
      positionOld.y = positionCurrent.y - reflectedVelY;
    }
  }
}

void Entity::handleEntityCollision(Entity &entity) {
  float minDist = radius + entity.radius;
  float xDist = positionCurrent.x - entity.positionCurrent.x;
  float yDist = positionCurrent.y - entity.positionCurrent.y;
  float dist = std::sqrt(xDist * xDist + yDist * yDist);

  if (dist <= minDist) {
    float nx = 0.0;
    float ny = 0.0;

    if (dist > 0.0001f) {
      nx = xDist / dist;
      ny = yDist / dist;
    } else {
      nx = GetRandomValue(-100, 100) / 100.0f;
      ny = GetRandomValue(-100, 100) / 100.0f;
      float randNormLength = std::sqrt(nx * nx + ny * ny);
      nx /= randNormLength;
      ny /= randNormLength;
    }

    Vector2 velA = Vector2{positionCurrent.x - positionOld.x,
                           positionCurrent.y - positionOld.y};
    Vector2 velB = Vector2{entity.positionCurrent.x - entity.positionOld.x,
                           entity.positionCurrent.y - entity.positionOld.y};

    float overlap = minDist - dist;

    if (overlap > MIN_CORRECTION) {
      float totalMass = mass + entity.mass;

      positionCurrent.x -= nx * overlap * (entity.mass / totalMass) * 1.005;
      positionCurrent.y -= ny * overlap * (entity.mass / totalMass) * 1.005;
      entity.positionCurrent.x += nx * overlap * (mass / totalMass) * 1.005;
      entity.positionCurrent.y += ny * overlap * (mass / totalMass) * 1.005;
    }

    float relVelX = velA.x - velB.x;
    float relVelY = velA.y - velB.y;
    float relVelDotNormal = velA.x * nx + relVelY * ny;

    if (relVelDotNormal < 0) {
      float relVelMagnitude = std::sqrt(relVelX * relVelX + relVelY * relVelY);
      float restitution = 0.6;

      if (relVelMagnitude < 0.5) {
        restitution *= (relVelMagnitude * 2.0);
      }

      float impluse = -(1.0 + restitution) * relVelDotNormal;
      impluse /= (1.0 / mass) + (1.0 / entity.mass);

      float impulseX = impluse * nx / mass;
      float impulseY = impluse * ny / mass;

      positionOld.x = positionCurrent.x + (velA.x - impulseX);
      positionOld.y = positionCurrent.y + (velA.y - impulseY);

      entity.positionOld.x =
          entity.positionCurrent.x + (velB.x + impulseX * (mass / entity.mass));
      entity.positionOld.y =
          entity.positionCurrent.y + (velB.y + impulseY * (mass / entity.mass));
    }
  }
}

void Entity::draw() {
  DrawCircle(positionCurrent.x, positionCurrent.y, radius, color);
}

void Entity::render(float deltaTime) {
  updatePosition(deltaTime);
  handleBoundaryCollision();
  draw();
}