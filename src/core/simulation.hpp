#pragma once

#include "core/constraint.hpp"
#include "core/object.hpp"
#include "raylib.h"
#include <map>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

struct PairHash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const;
};

struct Grid {
  float size;
  int cols, rows;
  std::map<std::pair<int, int>, std::vector<std::shared_ptr<CircularObject>>>
      grid;

  Grid(float size, float screenWidth, float screenHeight);

  void insert(std::shared_ptr<CircularObject> object);
  void clear();
  void getNeighbours(int row, int col,
                     std::vector<std::shared_ptr<CircularObject>> &out_objs);
};

struct MouseDragState {
  Vector2 *mouseDragStart;
  bool isDragging;
};

class Simulation {
public:
  Simulation(float screenWidth, float screenHeight,
             std::vector<std::shared_ptr<Object>> objects,
             std::shared_ptr<Constraint> constraint, bool useGridHashing,
             float gridHashingSize);
  void update(float dt);
  void render();
  void handleInput();
  void draw();

  std::vector<std::shared_ptr<Object>> objects;

private:
  void handleCollisions();
  void handleObjObjCollision(std::shared_ptr<Object> obj1,
                             std::shared_ptr<Object> obj2);
  void handleCircleCircleCollision(CircularObject *cobj1,
                                   CircularObject *cobj2);
  void handleCircleLineCollision(CircularObject *c, LineSegmentObject *l);

  float screenWidth;
  float screenHeight;
  bool useGridHashing;
  std::optional<Grid> grid;
  std::shared_ptr<Constraint> constraint;
  MouseDragState mouseDragState;
};
