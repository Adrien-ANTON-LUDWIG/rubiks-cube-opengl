#pragma once

#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
 
// Debug
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "cube.h"

#define AXIS_X glm::vec3(1, 0, 0)
#define AXIS_Y glm::vec3(0, 1, 0)
#define AXIS_Z glm::vec3(0, 0, 1)

class Cube {
 public:
  glm::vec4 origin_center = glm::vec4(0.0, 0.0, 0.0, 1.0); // Homographic coordinates
  glm::vec4 current_center = origin_center;
  glm::mat4 transform = glm::mat4(1.0f);
  std::vector<GLfloat> vertices = cube_vertices;

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    for (size_t i = 0; i < vertices.size(); i += 3) {
      vertices[i] += x;
      vertices[i + 1] += y;
      vertices[i + 2] += z;
    }

    origin_center[0] += x;
    origin_center[1] += y;
    origin_center[2] += z;
    current_center = origin_center;
  }

  void rotate(const float angle, const glm::vec3 &rotationAxis) {
    float angleRad = glm::radians(angle);
    transform = glm::rotate(glm::mat4(1.0), angleRad, rotationAxis) * transform;
    current_center = transform * origin_center;
  }
};

class RubiksCube {
 public:
  RubiksCube() {
    cubes = std::vector<Cube>(26);
    int i = 0;
    for (int x = -2; x <= 2; x += 2)
      for (int y = -2; y <= 2; y += 2)
        for (int z = -2; z <= 2; z += 2) {
          if (x == 0 && y == 0 && z == 0) continue;
          cubes[i].translate(x, y, z);
          i++;
        }
  }

  void translate(GLfloat x, GLfloat y, GLfloat z);
  void rotate(const float angle, const glm::vec3 &rotationAxis);
  void rotate_face();
  //void rotate_face(glm::vec3 axis);

  std::vector<Cube> cubes;
};