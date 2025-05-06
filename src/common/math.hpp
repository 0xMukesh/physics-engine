#pragma once

#include "raylib.h"

class Vec2 {
public:
  float x;
  float y;

  Vec2(float x, float y);

  float length();
  Vec2 operator+(const Vec2 &v) const;
  Vec2 operator-(const Vec2 &v) const;
  
  Vec2 operator*(float n) const;
  friend Vec2 operator*(float n, const Vec2 &v);
  Vec2 operator/(float n) const;
};

Vec2 Vector2ToVec2(Vector2 &v);