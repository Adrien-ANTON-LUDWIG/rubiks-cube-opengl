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

void RubiksCube::rotate_face() {
  std::cout << "Rotating" << std::endl;
  for (size_t i = 0; i < cubes.size(); i++) {
    std::cout << "RC: " << glm::to_string(cubes[i].current_center) << std::endl;
    if (my_float_comparison(cubes[i].current_center.x, 2.0))
        cubes[i].rotate(90, glm::vec3(1, 0, 0));
  }
}

// void RubiksCube::rotate_face(glm::vec3 axis) {
  // std::cout << "Rotating";
  // glm::vec4 axis4 = glm::vec4(axis, 0);
  // for (size_t i = 0; i < cubes.size(); i++) {
    // float axis_value = glm::dot(cubes[i].current_center, axis4);
    // std::cout << axis_value << std::endl;
    // if (my_float_comparison(axis_value, 2.0))
      // cubes[i].rotate(90, axis);
  // }
// }
// 