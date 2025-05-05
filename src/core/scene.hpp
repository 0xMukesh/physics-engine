#pragma once

#include "core/entity.hpp"
#include <vector>

class Scene {
public:
  Vector2 bounds;
  std::vector<Entity> entities;

  Scene(std::vector<Entity> entities, Vector2 bounds);
  void update();
  void render(float deltaTime);

private:
  void handleInput();
};