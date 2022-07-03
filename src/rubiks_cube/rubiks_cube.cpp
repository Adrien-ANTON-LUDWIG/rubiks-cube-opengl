#include "rubiks_cube.h"

#include <iostream>

void RubiksCube::translate(GLfloat x, GLfloat y, GLfloat z) {
  for (size_t i = 0; i < cubes.size(); i++) cubes[i].translate(x, y, z);
}

void RubiksCube::rotate(const float angle, const glm::vec3 &rotationAxis) {
  for (size_t i = 0; i < cubes.size(); i++) cubes[i].rotate(angle, rotationAxis);
}
