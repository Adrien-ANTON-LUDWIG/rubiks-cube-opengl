#include <GL/glew.h>
// KEEP GLEW HERE ^ (first include)

#include "core/gl.h"

int main(int argc, char *argv[]) {
  begin = std::chrono::steady_clock::now();

  if (!init_glut(argc, argv)) throw new std::runtime_error("Glut error");
  if (!init_glew()) throw new std::runtime_error("Glew error");
  if (!init_GL()) throw new std::runtime_error("Gl error");
  program = init_program();

  if (!init_object(program)) throw new std::runtime_error("Object error");

  // Set initial camera position
  update_position();
  glutMainLoop();

  return 0;
}
