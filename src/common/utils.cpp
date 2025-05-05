#include "utils.hpp"
#include <math.h>

float utils::clipNumber(float value, float threshold) {
  return (std::abs(value) < threshold) ? 0.0f : value;
}
