#pragma once

#include <GL/freeglut.h>

#include <vector>

#include "cube.h"

class Cube {
 public:
  std::vector<GLfloat> center = {0.0, 0.0, 0.0};
  std::vector<GLfloat> vertices = cube_vertices;

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    for (size_t i = 0; i < vertices.size(); i += 3) {
      vertices[i] += x;
      vertices[i + 1] += y;
      vertices[i + 2] += z;
    }

    center[0] += x;
    center[1] += y;
    center[2] += z;
  }
};

class RubiksCube {
 public:
  RubiksCube() {
    cubes = std::vector<Cube>(26);
    int i = 0;
    for (int x = -2; x <= 2; x+=2)
      for (int y = -2; y <= 2; y+=2)
        for (int z = -2; z <= 2; z+=2) {
          if (x == 0 && y == 0 && z == 0) continue;
          cubes[i].translate(x, y, z);
          i++;
        }
  }

  void init();

  std::vector<Cube> cubes;
};