#include "scene.hpp"
#include "common/constants.hpp"
#include "core/constraint.hpp"
#include "core/entity.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

Scene::Scene(std::vector<Entity> entities, Vector2 bounds)
    : bounds(bounds), entities(entities) {}

void Scene::update() {
  for (size_t i = 0; i < entities.size(); i++) {
    for (size_t j = i + 1; j < entities.size(); j++) {
      entities[i].handleEntityCollision(entities[j]);
    }
  }
}

void Scene::render(float deltaTime) {
  handleInput();

  for (auto &entity : entities) {
    entity.render(deltaTime);
  }
}

void Scene::handleInput() {
  Vector2 mousePosition = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    entities.push_back(Entity(
        0.5, 10, mousePosition, Vector2{0, ACCELERATION_DUE_TO_GRAVITY},
        std::make_shared<RectangularConstraint>(bounds.x, bounds.y), RED));
  }
}
