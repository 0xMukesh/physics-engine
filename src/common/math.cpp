#include "math.hpp"
#include <cmath>
#include <iostream>
#include <ostream>

Vec2::Vec2(float x, float y) : x(x), y(y) {}

float Vec2::length() { return std::sqrt(x * x + y * y); }

Vec2 Vec2::operator+(const Vec2 &v) const { return Vec2(x + v.x, y + v.y); }
Vec2 Vec2::operator-(const Vec2 &v) const { return Vec2(x - v.x, y - v.y); }
Vec2 Vec2::operator*(float n) const { return Vec2(x * n, y * n); }
Vec2 Vec2::operator/(float n) const {
  if (n == 0) {
    std::cerr << "cannot divide by 0" << std::endl;
    throw std::runtime_error("division by zero in Vec2::operator/");
  }
  return Vec2(x / n, y / n);
}
Vec2 operator*(float n, const Vec2 &v) { return v * n; }

Vec2 Vector2ToVec2(Vector2 &v) { return Vec2(v.x, v.y); }
Vector2 Vec2ToVector2(Vec2 &v) { return Vector2{v.x, v.y}; }
