#include "scene.hpp"
#include "common/constants.hpp"
#include "core/entity.hpp"
#include "raylib.h"
#include <vector>

Scene::Scene(std::vector<Entity> entities, Vector2 bounds)
    : bounds(bounds), entities(entities), selectedEntity(nullptr) {}

void Scene::update() {
  for (size_t i = 0; i < entities.size(); i++) {
    for (size_t j = 0; j < entities.size(); j++) {
      if (&entities[i] == &entities[j])
        continue;

      entities[i].handleEntityCollision(entities[j]);
    }
  }
}

void Scene::render(float deltaTime) {
  handleKeyboardInput();

  for (auto &entity : entities) {
    entity.render(deltaTime);
  }
}

void Scene::handleKeyboardInput() {
  Vector2 mousePosition = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    for (auto &entity : entities) {
      entity.isSelected = false;
      selectedEntity = nullptr;
    }

    for (auto &entity : entities) {
      bool isMouseWithinEntityBounds = CheckCollisionCircles(
          entity.position, entity.radius, mousePosition, 0.0001f);

      if (isMouseWithinEntityBounds) {
        entity.isSelected = true;
        selectedEntity = &entity;
        break;
      }
    }
  }

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && selectedEntity == nullptr) {
    entities.push_back(Entity(0.5, 10.0, mousePosition, Vector2{0, 0},
                              Vector2{0, ACCELERATION_DUE_TO_GRAVITY}, bounds,
                              RED));
  }

  if (selectedEntity != nullptr) {
    if (IsKeyDown(KEY_LEFT)) {
      selectedEntity->position.x -= 3;
    }

    if (IsKeyDown(KEY_RIGHT)) {
      selectedEntity->position.x += 3;
    }
    if (IsKeyDown(KEY_UP)) {
      selectedEntity->position.y -= 3;
    }
    if (IsKeyDown(KEY_DOWN)) {
      selectedEntity->position.y += 3;
    }
  }
}
