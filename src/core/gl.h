#pragma once

#include "../io/camera.h"
#include "../io/keyboard.h"
#include "../io/mouse.h"
#include "../rubiks_cube/rubiks_cube.h"
#include "program.h"
#include "test_error.h"

#define PI 3.14159265358979323846

bool init_object(Program *program) {
  for (size_t i = 0; i < rubiks_cube.cubes.size(); i++) {
    int max_nb_vbo = 2;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location =
        glGetAttribLocation(program->program_id, "position");
    TEST_OPENGL_ERROR();
    GLint color_location = glGetAttribLocation(program->program_id, "color");
    TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &rubiks_cube.cubes[i].vao_id);
    TEST_OPENGL_ERROR();
    glBindVertexArray(rubiks_cube.cubes[i].vao_id);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1) nb_vbo++;
    if (color_location != -1) nb_vbo++;
    glGenBuffers(nb_vbo, vbo_ids);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1) {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
      TEST_OPENGL_ERROR();
      glBufferData(GL_ARRAY_BUFFER,
                   rubiks_cube.cubes[i].vertices.size() * sizeof(float),
                   rubiks_cube.cubes[i].vertices.data(), GL_STATIC_DRAW);
      TEST_OPENGL_ERROR();
      glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
      TEST_OPENGL_ERROR();
      glEnableVertexAttribArray(vertex_location);
      TEST_OPENGL_ERROR();
    }

    if (color_location != -1) {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
      TEST_OPENGL_ERROR();
      glBufferData(GL_ARRAY_BUFFER, cube_color.size() * sizeof(float),
                   cube_color.data(), GL_STATIC_DRAW);
      TEST_OPENGL_ERROR();
      glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
      TEST_OPENGL_ERROR();
      glEnableVertexAttribArray(color_location);
      TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);
  }
  return true;
}

#include <algorithm>

struct compare_cube {
  inline bool operator()(const Cube &cube1, const Cube &cube2) const {
    return (glm::length(camera_position - cube1.current_center) >
            glm::length(camera_position - cube2.current_center));
  }
};

void display() {
  static int counter = 0;
  counter++;

  glUseProgram(program->program_id);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  TEST_OPENGL_ERROR();

  std::sort(rubiks_cube.cubes.begin(), rubiks_cube.cubes.end(), compare_cube());

  auto elapsed = rubiks_cube.update_status();

  // Draw back faces
  {
    glCullFace(GL_FRONT);
    TEST_OPENGL_ERROR();

    for (size_t i = 0; i < rubiks_cube.cubes.size(); i++) {
      glBindVertexArray(rubiks_cube.cubes[i].vao_id);
      TEST_OPENGL_ERROR();

      // Pass the cube transformation matrix to the vertex shader
      GLuint transform_location =
          glGetUniformLocation(program->program_id, "transform");

      glUniformMatrix4fv(
          transform_location, 1, GL_FALSE,
          glm::value_ptr(rubiks_cube.cubes[i].get_transform(elapsed)));

      glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size() / 3);
      TEST_OPENGL_ERROR();
    }
  }

  // Draw front faces
  {
    glCullFace(GL_BACK);
    TEST_OPENGL_ERROR();

    for (size_t i = 0; i < rubiks_cube.cubes.size(); i++) {
      glBindVertexArray(rubiks_cube.cubes[i].vao_id);
      TEST_OPENGL_ERROR();

      // Pass the cube transformation matrix to the vertex shader
      GLuint transform_location =
          glGetUniformLocation(program->program_id, "transform");

      glUniformMatrix4fv(
          transform_location, 1, GL_FALSE,
          glm::value_ptr(rubiks_cube.cubes[i].get_transform(elapsed)));

      glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size() / 3);
      TEST_OPENGL_ERROR();
    }
  }

  glBindVertexArray(0);
  TEST_OPENGL_ERROR();
  glutSwapBuffers();

  if (rubiks_cube.rotating) {
    glutPostRedisplay();
    TEST_OPENGL_ERROR();
  }
}

void window_resize(int width, int height) {
  // std::cout << "glViewport(0,0,"<< width << "," << height <<
  // ");TEST_OPENGL_ERROR();" << std::endl;
  glViewport(0, 0, width, height);
  TEST_OPENGL_ERROR();
}

bool init_glut(int &argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitContextVersion(4, 5);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1024, 1024);
  glutInitWindowPosition(10, 10);
  glutCreateWindow("Test OpenGL - POGL");

  glutDisplayFunc(display);
  glutReshapeFunc(window_resize);
  glutMouseFunc(mouse_callback);
  glutMotionFunc(mouse_motion_callback);

  glutKeyboardFunc(keyboard_normal_callback);

  return true;
}

bool init_glew() { return (glewInit() == GLEW_OK); }

bool init_GL() {
  // Set background color
  glClearColor(0.4, 0.4, 0.4, 1.0);
  TEST_OPENGL_ERROR();

  // Enable blending
  glEnable(GL_BLEND);
  TEST_OPENGL_ERROR();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  TEST_OPENGL_ERROR();

  // Backface culling
  glEnable(GL_CULL_FACE);
  TEST_OPENGL_ERROR();

  return true;
}
