#pragma once

#include <GL/freeglut.h>
#include <string.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>

void test_opengl_error(std::string func, std::string file, int line) {
  GLenum err = glGetError();
  switch (err) {
    case GL_NO_ERROR:
      return;
    case GL_INVALID_ENUM:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "GL_INVALID_ENUM\n";
      break;
    case GL_INVALID_VALUE:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "GL_INVALID_VALUE\n";
      break;
    case GL_INVALID_OPERATION:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "GL_INVALID_OPERATION\n";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION\n";
      break;
    case GL_OUT_OF_MEMORY:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "GL_OUT_OF_MEMORY\n";
      break;
    case GL_STACK_UNDERFLOW:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "GL_STACK_UNDERFLOW\n";
      break;
    case GL_STACK_OVERFLOW:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "GL_STACK_OVERFLOW\n";
      break;
    default:
      std::cerr << file << ":" << line << "(" << func << ") ";
      std::cerr << "UNKONWN ERROR\n";
      break;
  }
}

#define TEST_OPENGL_ERROR()                          \
  do {                                               \
    test_opengl_error(__func__, __FILE__, __LINE__); \
  } while (0)

