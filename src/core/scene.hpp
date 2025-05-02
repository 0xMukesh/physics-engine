#pragma once

#include "core/entity.hpp"
#include <vector>

class Scene {
public:
  std::vector<Entity> entities;
  Entity *selectedEntity;

  Scene(std::vector<Entity> entities);
  void render();
  void handleKeyboardInput();
};