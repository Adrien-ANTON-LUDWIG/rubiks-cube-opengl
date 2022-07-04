#include "rubiks_cube.h"

#include <iostream>

#define EPSILON 0.001

inline bool my_float_comparison(float a, float b) {
    return abs(a - b) < EPSILON;
}

void RubiksCube::translate(GLfloat x, GLfloat y, GLfloat z) {
  for (size_t i = 0; i < cubes.size(); i++) cubes[i].translate(x, y, z);
}

void RubiksCube::rotate(const float angle, const glm::vec3 &rotationAxis) {
  for (size_t i = 0; i < cubes.size(); i++) cubes[i].rotate(angle, rotationAxis);
}

void RubiksCube::rotate_face() {
  for (size_t i = 0; i < cubes.size(); i++)
    if (my_float_comparison(cubes[i].current_center.x, 2.0))
        cubes[i].rotate(90, glm::vec3(1, 0, 0));
}
