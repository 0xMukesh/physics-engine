#include "scene.hpp"
#include "core/entity.hpp"
#include "raylib.h"
#include <vector>

Scene::Scene(std::vector<Entity> entities)
    : entities(entities), selectedEntity(nullptr) {}

void Scene::render() {
  Vector2 mousePosition = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    for (auto &entity : entities) {
      entity.isSelected = false;
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

  handleKeyboardInput();

  for (auto &entity : entities) {
    entity.render();
  }
}

void Scene::handleKeyboardInput() {
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
