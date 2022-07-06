#pragma once

#include <glob.h>

#include "../io/camera.h"
#include "../io/keyboard.h"
#include "../io/mouse.h"
#include "../rubiks_cube/rubiks_cube.h"
#include "program.h"
#include "test_error.h"

extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
}

#define PI 3.14159265358979323846
#define GLOB_TEXTURES "./textures/*"

void init_textures() {
  // load and generate texture
  int width;
  int height;
  int channels;

  glob_t glob_result;
  glob(GLOB_TEXTURES, GLOB_TILDE, NULL, &glob_result);
  for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data =
        stbi_load(glob_result.gl_pathv[i], &width, &height, &channels, 0);

    if (!data) {
      std::cout << "Failed to load texture.\n";
      throw;
    }

    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    TEST_OPENGL_ERROR();

    rubiks_cube.texture_ids.push_back(texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    TEST_OPENGL_ERROR();
    glGenerateMipmap(GL_TEXTURE_2D);
    TEST_OPENGL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();

    stbi_image_free(data);
  }

  active_texture(rubiks_cube.get_next_texture_id());
}

bool init_object(Program *program) {
  for (size_t i = 0; i < rubiks_cube.cubes.size(); i++) {
    int max_nb_vbo = 3;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location =
        glGetAttribLocation(program->program_id, "position");
    TEST_OPENGL_ERROR();
    GLint color_location = glGetAttribLocation(program->program_id, "color");
    TEST_OPENGL_ERROR();
    GLint texture_location = glGetAttribLocation(program->program_id, "uv");
    TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &rubiks_cube.cubes[i].vao_id);
    TEST_OPENGL_ERROR();
    glBindVertexArray(rubiks_cube.cubes[i].vao_id);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1) nb_vbo++;
    if (color_location != -1) nb_vbo++;
    if (texture_location != -1) nb_vbo++;
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

    if (texture_location != -1) {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
      TEST_OPENGL_ERROR();
      glBufferData(GL_ARRAY_BUFFER, texture_uv.size() * sizeof(float),
                   texture_uv.data(), GL_STATIC_DRAW);
      TEST_OPENGL_ERROR();
      glVertexAttribPointer(texture_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
      TEST_OPENGL_ERROR();
      glEnableVertexAttribArray(texture_location);
      TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);
  }
  return true;
}

#include <algorithm>

struct compare_cube {
  // Comparator to sort in reverse order
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

  // Sort cubes to draw them from the farthest to the closest.
  std::sort(rubiks_cube.cubes.begin(), rubiks_cube.cubes.end(), compare_cube());

  auto elapsed = rubiks_cube.update_status();

  // Draw front faces
  for (size_t i = 0; i < rubiks_cube.cubes.size(); i++) {
    glBindVertexArray(rubiks_cube.cubes[i].vao_id);
    TEST_OPENGL_ERROR();

    // Pass the cube transformation matrix to the vertex shader
    GLuint transform_location =
        glGetUniformLocation(program->program_id, "transform");
    glUniformMatrix4fv(
        transform_location, 1, GL_FALSE,
        glm::value_ptr(rubiks_cube.cubes[i].get_transform(elapsed)));

    // Pass the cube opacity
    GLuint opacity_location =
        glGetUniformLocation(program->program_id, "opacity");
    glUniform1f(opacity_location, rubiks_cube.opacity);

    glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size() / 3);
    TEST_OPENGL_ERROR();
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

  // Update camera properties
  window_width = width;
  window_height = height;

  update_position();
  glutPostRedisplay();
  TEST_OPENGL_ERROR();
}

bool init_glut(int &argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitContextVersion(4, 5);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
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
  // Enable Depth test at init because we start with an opaque cube
  glEnable(GL_DEPTH_TEST);
  TEST_OPENGL_ERROR();

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
  glCullFace(GL_BACK);
  TEST_OPENGL_ERROR();

  return true;
}
