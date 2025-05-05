#include "entity.hpp"
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

    if (dist + radius > constraintRadius) {
      float nx = xDist / dist;
      float ny = yDist / dist;

      positionCurrent.x = constraintCenter.x + nx * (dist - radius);
      positionCurrent.y = constraintCenter.y + ny * (dist - radius);
    }
  }
}

void Entity::handleEntityCollision(Entity &entity) {}

void Entity::draw() {
  DrawCircle(positionCurrent.x, positionCurrent.y, radius, color);
}

void Entity::render(float deltaTime) {
  updatePosition(deltaTime);
  handleBoundaryCollision();
  draw();
}