#include <GL/glew.h>
// KEEP GLEW HERE ^ (first include)

#include <GL/freeglut.h>
#include <string.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "utils/translate.h"

#define Z_NEAR 0.5
#define Z_FAR 100

class Program;

Program *program;
GLuint vao_id;

double distance = 50;
double angle_alpha = 0;
double angle_beta = 0;
double sky_up = 1;

int old_pos_x = 0;
int old_pos_y = 0;

std::vector<GLfloat> vertex_buffer_data = {};

void make_cylinder(float r, float z0, float z1, int N = 100) {
  float angle_step = 2 * M_PI / N;
  float last_x = r;
  float last_y = 0;
  // float centerx = 0;
  // float centery = 0;

  for (int i = 1; i <= N; i++) {
    float angle = angle_step * i;
    float x = r * cos(angle);
    float y = r * sin(angle);

    vertex_buffer_data.push_back(last_x);
    vertex_buffer_data.push_back(last_y);
    vertex_buffer_data.push_back(z0);

    vertex_buffer_data.push_back(last_x);
    vertex_buffer_data.push_back(last_y);
    vertex_buffer_data.push_back(z1);

    vertex_buffer_data.push_back(x);
    vertex_buffer_data.push_back(y);
    vertex_buffer_data.push_back(z0);

    vertex_buffer_data.push_back(x);
    vertex_buffer_data.push_back(y);
    vertex_buffer_data.push_back(z0);

    vertex_buffer_data.push_back(last_x);
    vertex_buffer_data.push_back(last_y);
    vertex_buffer_data.push_back(z1);

    vertex_buffer_data.push_back(x);
    vertex_buffer_data.push_back(y);
    vertex_buffer_data.push_back(z1);

    last_x = x;
    last_y = y;
  }
}

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const std::vector<GLfloat> cube_vertex_buffer_data = {
    // RED
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,

    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,

    // GREEN
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,

    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,

    // ORANGE
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,

    // BLUE
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,

    // YELLOW
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,

    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,

    // WHITE
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
};


static const std::vector<GLfloat> cube_color_buffer_data = {
    // Red
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,

    // Green 
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,

    // Orange
    1.0, 0.5, 0.0,
    1.0, 0.5, 0.0,
    1.0, 0.5, 0.0,
    1.0, 0.5, 0.0,
    1.0, 0.5, 0.0,
    1.0, 0.5, 0.0,

    // Blue
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    // Yellow
    1.0, 1.0, 0.0, 
    1.0, 1.0, 0.0, 
    1.0, 1.0, 0.0, 
    1.0, 1.0, 0.0, 
    1.0, 1.0, 0.0, 
    1.0, 1.0, 0.0, 

    // White
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
};


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

bool init_glew() { return (glewInit() == GLEW_OK); }

bool init_GL() {
  glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
  glClearColor(0.4, 0.4, 0.4, 1.0);
  TEST_OPENGL_ERROR();
  return true;
}

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

bool init_object(Program *program) {
  int max_nb_vbo = 5;
  int nb_vbo = 0;
  int index_vbo = 0;
  GLuint vbo_ids[max_nb_vbo];

  GLint vertex_location = glGetAttribLocation(program->program_id, "position");
  TEST_OPENGL_ERROR();
  // GLint normal_smooth_location = glGetAttribLocation(program->program_id, "normalSmooth");
  // TEST_OPENGL_ERROR();
  GLint color_location = glGetAttribLocation(program->program_id, "color");
  TEST_OPENGL_ERROR();

  glGenVertexArrays(1, &vao_id);
  TEST_OPENGL_ERROR();
  glBindVertexArray(vao_id);
  TEST_OPENGL_ERROR();

  if (vertex_location != -1) nb_vbo++;
  // if (normal_smooth_location != -1) nb_vbo++;
  if (color_location != -1) nb_vbo++;
  glGenBuffers(nb_vbo, vbo_ids);
  TEST_OPENGL_ERROR();

  if (vertex_location != -1) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float),
                 vertex_buffer_data.data(), GL_STATIC_DRAW);
    TEST_OPENGL_ERROR();
    glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(vertex_location);
    TEST_OPENGL_ERROR();
  }

  // if (normal_smooth_location != -1) {
  //   glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
  //   TEST_OPENGL_ERROR();
  //   glBufferData(GL_ARRAY_BUFFER,
  //                normal_smooth_buffer_data.size() * sizeof(float),
  //                normal_smooth_buffer_data.data(), GL_STATIC_DRAW);
  //   TEST_OPENGL_ERROR();
  //   glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0,
  //   0); TEST_OPENGL_ERROR();
  //   glEnableVertexAttribArray(normal_smooth_location);
  //   TEST_OPENGL_ERROR();
  // }

  if (color_location!=-1) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, cube_color_buffer_data.size()*sizeof(float), cube_color_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(color_location);TEST_OPENGL_ERROR();
  }



  glBindVertexArray(0);
  return true;
}

void display() {
  static int counter = 0;
  counter++;

  // static glm::vec3 red(1.0, 0.0, 0.0);
  // static glm::vec3 green(0.0, 1.0, 0.0);
  // static glm::vec3 blue(0.0, 0.0, 1.0);
  // static glm::vec3 colors[] = {red, green, blue};
  // GLuint color_location = glGetUniformLocation(program->program_id, "color");
  // glUniform3fv(color_location, 1, glm::value_ptr(colors[counter % 3]));

  glUseProgram(program->program_id);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  TEST_OPENGL_ERROR();
  glBindVertexArray(vao_id);
  TEST_OPENGL_ERROR();
  glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size() / 3);
  TEST_OPENGL_ERROR();
  glBindVertexArray(0);
  TEST_OPENGL_ERROR();
  glutSwapBuffers();
}

void frustum(GLfloat data[16], const float &left, const float &right,
             const float &bottom, const float &top, const float &z_near,
             const float &z_far) {
  float a = (right + left) / (right - left);
  float b = (top + bottom) / (top - bottom);
  float c = -(z_far + z_near) / (z_far - z_near);
  float d = -(2 * z_far * z_near) / (z_far - z_near);

  data[0] = 2 * z_near / (right - left);
  data[1] = 0;
  data[2] = a;
  data[3] = 0;

  data[4] = 0;
  data[5] = 2 * z_near / (top - bottom);
  data[6] = b;
  data[7] = 0;

  data[8] = 0;
  data[9] = 0;
  data[10] = c;
  data[11] = d;

  data[12] = 0;
  data[13] = 0;
  data[14] = -1;
  data[15] = 0;

  // std::cout << "data\n";
  // std::cout << data[0] << " " << data[1] << " " <<  data[2] << " " << data[3]
  // <<  "\n"; std::cout << data[4] << " " << data[5] << " " <<  data[6] << " "
  // <<   data[7] <<  "\n"; std::cout << data[8] << " " << data[9] << " " <<
  // data[10] << " " <<   data[11] <<  "\n"; std::cout << data[12] << " " <<
  // data[13] << " " <<  data[14] << " " <<   data[15] <<  "\n";
}

void look_at(GLfloat data[16], const float &eyeX, const float &eyeY,
             const float &eyeZ, const float &centerX, const float &centerY,
             const float &centerZ, float upX, float upY, float upZ) {
  float fx = centerX - eyeX;
  float fy = centerY - eyeY;
  float fz = centerZ - eyeZ;

  float sx, sy, sz;
  float sn;
  float ux, uy, uz;

  float nf = std::sqrt(fx * fx + fy * fy + fz * fz);

  float nu = std::sqrt(upX * upX + upY * upY + upZ * upZ);

  fx /= nf;
  fy /= nf;
  fz /= nf;

  upX /= nu;
  upY /= nu;
  upZ /= nu;

  sx = fy * upZ - fz * upY;
  sy = fz * upX - fx * upZ;
  sz = fx * upY - fy * upX;

  sn = std::sqrt(sx * sx + sy * sy + sz * sz);
  sx /= sn;
  sy /= sn;
  sz /= sn;

  ux = sy * fz - sz * fy;
  uy = sz * fx - sx * fz;
  uz = sx * fy - sy * fx;

  data[0] = sx;
  data[1] = sy;
  data[2] = sz;
  data[3] = 0;

  data[4] = ux;
  data[5] = uy;
  data[6] = uz;
  data[7] = 0;

  data[8] = -fx;
  data[9] = -fy;
  data[10] = -fz;
  data[11] = -distance;

  data[12] = 0;
  data[13] = 0;
  data[14] = 0;
  data[15] = 1;

  // std::cout << "data\n";
  // std::cout << data[0] << " " << data[1] << " " <<  data[2] << " " << data[3]
  // <<  "\n"; std::cout << data[4] << " " << data[5] << " " <<  data[6] << " "
  // <<   data[7] <<  "\n"; std::cout << data[8] << " " << data[9] << " " <<
  // data[10] << " " <<   data[11] <<  "\n"; std::cout << data[12] << " " <<
  // data[13] << " " <<  data[14] << " " <<   data[15] <<  "\n"; std::cout <<
  // "\n"; std::cout << "angles " << angle_alpha <<  " "  << angle_beta << "\n";
  // std::cout << "distance " << distance << "\n";
  // std::cout << "eye " << eyeX  << ", " << eyeY << ", " << eyeZ << std::endl;
  // std::cout << "center " << centerX  << ", " << centerY << ", " << centerZ <<
  // std::endl;
}

void update_position() {
  GLfloat model_view_matrix[16];
  GLfloat projection_matrix[16];

  double p0 = distance * cos(angle_alpha) * cos(angle_beta);
  double p1 = distance * sin(angle_beta);
  double p2 = distance * sin(angle_alpha) * cos(angle_beta);

  look_at(model_view_matrix, p0, p1, p2, 0, 0, 0, 0, sky_up, 0);
  glUseProgram(program->program_id);
  GLint model_view_matrix_location =
      glGetUniformLocation(program->program_id, "model_view_matrix");

  // std::cout << "model_view_matrix_location " << model_view_matrix_location <<
  // std::endl;
  glUniformMatrix4fv(model_view_matrix_location, 1, GL_TRUE, model_view_matrix);

  frustum(projection_matrix, -0.05, 0.05, -0.05, 0.05, Z_NEAR, Z_FAR);
  GLint projection_matrix_location =
      glGetUniformLocation(program->program_id, "projection_matrix");
  glUniformMatrix4fv(projection_matrix_location, 1, GL_TRUE, projection_matrix);
  // std::cout << "projection_matrix_location " << projection_matrix_location <<
  // std::endl;
}

void mouse_motion_callback(int x, int y) {
  double alpha = angle_alpha - (old_pos_x - x) * M_PI / 50.0;
  double beta = angle_beta - (old_pos_y - y) * M_PI / 50.0;
  double sky;

  if (beta > M_PI / 2.0) {
    beta = M_PI / 2.0;
    sky = -1;
  } else if (beta < -M_PI / 2.0) {
    beta = -M_PI / 2.0;
    sky = -1;
  } else
    sky = 1;

  if (alpha > M_PI) alpha -= 2 * M_PI;
  if (alpha < 0) alpha += 2 * M_PI;

  angle_alpha = alpha;
  angle_beta = beta;
  sky_up = sky;

  old_pos_x = x;
  old_pos_y = y;
  //  std::cout << "motion" << std::endl;
  update_position();
  glutPostRedisplay();
}

void mouse_wheel_callback(int, int dir, int, int) {
  if (dir < 0) {
    distance--;
    if (distance < 5) distance = 5;
  } else if (dir > 0) {
    distance++;
    if (distance > 50) distance = 50;
  }
  update_position();
  glutPostRedisplay();
}

void mouse_callback(int button, int, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    old_pos_x = x;
    old_pos_y = y;
  }
  if (button == 3)
    mouse_wheel_callback(button, -1, x, y);
  else if (button == 4)
    mouse_wheel_callback(button, 1, x, y);
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

  return true;
}

int main(int argc, char *argv[]) {
  if (!init_glut(argc, argv)) throw new std::runtime_error("Glut error");
  if (!init_glew()) throw new std::runtime_error("Glew error");
  if (!init_GL()) throw new std::runtime_error("Gl error");
  program = init_program();

  vertex_buffer_data = cube_vertex_buffer_data;
  vertex_buffer_data =  translate(vertex_buffer_data, 1, 0, 0);

  if (!init_object(program)) throw new std::runtime_error("Object error");


  glutMainLoop();

  return 0;
}
