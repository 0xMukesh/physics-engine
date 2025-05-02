#include "utils.hpp"

float utils::clipNumber(float number, float threshold) {
  return number <= threshold ? 0.0 : number;
}
