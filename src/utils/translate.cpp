#include "translate.h"

std::vector<GLfloat> translate(std::vector<GLfloat> object, float x, float y, float z) {
  for (size_t i = 0; i < object.size(); i++)
  {
    object[i] += x;
    object[i + 1] += y;
    object[i + 2] += z;
  }

  return object;
}

