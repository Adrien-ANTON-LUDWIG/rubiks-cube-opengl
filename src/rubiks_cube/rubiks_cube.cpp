#include "rubiks_cube.h"

#include <iostream>

#define EPSILON 0.1

inline bool my_float_comparison(float a, float b) {
  return abs(a - b) < EPSILON;
}

void RubiksCube::translate(GLfloat x, GLfloat y, GLfloat z) {
  for (size_t i = 0; i < cubes.size(); i++) cubes[i].translate(x, y, z);
}

void RubiksCube::rotate(const float angle, const glm::vec3 &rotationAxis) {
  for (size_t i = 0; i < cubes.size(); i++)
    cubes[i].rotate(angle, rotationAxis);
}

void RubiksCube::rotate_face(float angle, glm::vec3 axis) {
  glm::vec4 axis4 = glm::vec4(axis, 0);
  for (size_t i = 0; i < cubes.size(); i++) {
    float axis_value = glm::dot(cubes[i].current_center, axis4);
    if (my_float_comparison(axis_value, 2.0)) cubes[i].rotate(angle, axis);
  }
}
