#pragma once

#include "test_error.h"

class Program;
static Program *program;

std::string load(const std::string &filename) {
  std::ifstream input_src_file(filename, std::ios::in);
  std::string ligne;
  std::string file_content = "";
  if (input_src_file.fail()) {
    std::cerr << "FAIL\n";
    return "";
  }
  while (getline(input_src_file, ligne)) {
    file_content = file_content + ligne + "\n";
  }
  file_content += '\0';
  input_src_file.close();
  return file_content;
}

class Program {
 public:
  Program() {}
  ~Program() {}
  static Program *make_program(std::string &vertex_shader_src,
                               std::string &fragment_shader_src) {
    Program *program = new Program();

    // Init
    std::string vertex_src = load(vertex_shader_src);
    std::string fragment_src = load(fragment_shader_src);
    GLuint shader_id[2];
    GLint compile_status = GL_TRUE;
    char *vertex_shd_src =
        (char *)std::malloc(vertex_src.length() * sizeof(char));
    char *fragment_shd_src =
        (char *)std::malloc(fragment_src.length() * sizeof(char));
    vertex_src.copy(vertex_shd_src, vertex_src.length());
    fragment_src.copy(fragment_shd_src, fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);
    TEST_OPENGL_ERROR();
    shader_id[1] = glCreateShader(GL_FRAGMENT_SHADER);
    TEST_OPENGL_ERROR();

    glShaderSource(shader_id[0], 1, (const GLchar **)&(vertex_shd_src), 0);
    TEST_OPENGL_ERROR();
    glShaderSource(shader_id[1], 1, (const GLchar **)&(fragment_shd_src), 0);
    TEST_OPENGL_ERROR();

    // Compile
    for (int i = 0; i < 2; i++) {
      glCompileShader(shader_id[i]);
      TEST_OPENGL_ERROR();
      glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
      if (compile_status != GL_TRUE) {
        GLint log_size;
        char *shader_log;
        glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
        shader_log = (char *)std::malloc(
            log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
        if (shader_log != 0) {
          glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);
          std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
          std::free(shader_log);
        }
        std::free(vertex_shd_src);
        std::free(fragment_shd_src);
        glDeleteShader(shader_id[0]);
        glDeleteShader(shader_id[1]);
        return program;
      }
    }
    std::free(vertex_shd_src);
    std::free(fragment_shd_src);

    // LINK
    GLint link_status = GL_TRUE;
    program->program_id = glCreateProgram();
    TEST_OPENGL_ERROR();
    if (program->program_id == 0) return program;
    for (int i = 0; i < 2; i++) {
      glAttachShader(program->program_id, shader_id[i]);
      TEST_OPENGL_ERROR();
    }
    glLinkProgram(program->program_id);
    TEST_OPENGL_ERROR();
    glGetProgramiv(program->program_id, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
      GLint log_size;
      char *program_log;
      glGetProgramiv(program->program_id, GL_INFO_LOG_LENGTH, &log_size);
      program_log = (char *)std::malloc(
          log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
      if (program_log != 0) {
        glGetProgramInfoLog(program->program_id, log_size, &log_size,
                            program_log);
        std::cerr << "Program " << program_log << std::endl;
        std::free(program_log);
      }
      glDeleteProgram(program->program_id);
      TEST_OPENGL_ERROR();
      glDeleteShader(shader_id[0]);
      TEST_OPENGL_ERROR();
      glDeleteShader(shader_id[1]);
      TEST_OPENGL_ERROR();
      program->program_id = 0;
      return program;
    }
    return program;
  }

  char *get_log() { throw new std::logic_error("Not implemented yet."); }
  bool is_ready() { return program_id != 0; }
  void use() {
    glUseProgram(program_id);
    TEST_OPENGL_ERROR();
  }

  GLuint program_id = 0;
};

Program *init_program() {
  std::string vertex_shd = "vertex.shader";
  std::string fragment_shd = "fragment.shader";
  Program *program = Program::make_program(vertex_shd, fragment_shd);
  if (!program->is_ready()) throw new std::runtime_error("Program error");
  program->use();
  return program;
}
