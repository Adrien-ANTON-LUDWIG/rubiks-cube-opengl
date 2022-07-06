#pragma once

#include <GL/freeglut.h>

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Debug
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "cube.h"

#define DURATION 600.0f  // In milliseconds (=1s)

#define AXIS_X glm::vec3(1, 0, 0)
#define AXIS_Y glm::vec3(0, 1, 0)
#define AXIS_Z glm::vec3(0, 0, 1)

#define ANGLE_CLOCKWISE 90
#define ANGLE_COUNTER_CLOCKWISE -90

inline void print_matrix(glm::mat4 m) {
  for (int i = 0; i < 4; i++) std::cout << glm::to_string(m[i]) << std::endl;
}

class Cube {
 public:
  glm::vec4 origin_center =
      glm::vec4(0.0, 0.0, 0.0, 1.0);  // Homographic coordinates
  glm::vec4 current_center = origin_center;
  glm::mat4 transform = glm::mat4(1.0f);
  std::vector<GLfloat> vertices = cube_vertices;
  bool rotating = false;
  GLuint vao_id;

  glm::vec3 last_axis = glm::vec3(0.0f);
  float last_angle = 0;

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

  glm::mat4 get_transform(float elapsed) {
    if (!rotating) {
      return transform;
    }

    if (elapsed >= DURATION) {
      rotating = false;
      current_center = transform * origin_center;
      return transform;
    }

    // else
    float angle = last_angle / (DURATION / elapsed);

    // (last_angle - angle) because we already coputed the goal
    // position. We compute the steps between the last
    // position and the current position.
    glm::mat4 interpTransform =
        glm::rotate(glm::mat4(1.0f), glm::radians(-last_angle + angle),
                    last_axis) *
        transform;
    current_center = interpTransform * origin_center;

    return interpTransform;
  }

  void rotate(const float angle, const glm::vec3 &rotation_axis) {
    // Save current rotation for 'get_transform'
    last_axis = rotation_axis;
    last_angle = angle;
    rotating = true;

    // Compute new transform matrix for rotation
    float angleRad = glm::radians(angle);
    transform =
        glm::rotate(glm::mat4(1.0f), angleRad, rotation_axis) * transform;
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
  void rotate_face(float angle, glm::vec3 axis);
  float update_status();

  std::vector<Cube> cubes;
  std::chrono::steady_clock::time_point begin;
  bool rotating = false;
  float opacity = 1.0;
};

static RubiksCube rubiks_cube;
