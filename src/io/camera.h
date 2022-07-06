#pragma once

//#include <cmath>

#include "../core/program.h"

#define PI 3.14159265358979323846
#define Z_NEAR 0.5
#define Z_FAR 200
#define INITIAL_ANGLE_ALPHA 60.5 * PI / 180  // 60.5° to radians
#define INITIAL_ANGLE_BETA 24 * PI / 180   // 24° to radians

double distance = 15;
double angle_alpha = INITIAL_ANGLE_ALPHA;
double angle_beta = INITIAL_ANGLE_BETA;
double sky_up = 1;
glm::vec4 camera_position;

int old_pos_x = 0;
int old_pos_y = 0;

float window_width = 1024;
float window_height = 1024;

float fov = 45 * PI / 180; // 60° to radians

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
}

void update_position() {
  GLfloat model_view_matrix[16];
  GLfloat projection_matrix[16];

  double p0 = distance * cos(angle_alpha) * cos(angle_beta);
  double p1 = distance * sin(angle_beta);
  double p2 = distance * sin(angle_alpha) * cos(angle_beta);
  camera_position = glm::vec4(p0, p1, p2, 1.0);

  look_at(model_view_matrix, p0, p1, p2, 0, 0, 0, 0, sky_up, 0);
  glUseProgram(program->program_id);
  GLint model_view_matrix_location =
      glGetUniformLocation(program->program_id, "model_view_matrix");

  // std::cout << "model_view_matrix_location " << model_view_matrix_location <<
  // std::endl;
  glUniformMatrix4fv(model_view_matrix_location, 1, GL_TRUE, model_view_matrix);

  float aspect = (float) window_width / (float) window_height;
  float top = tan(fov * 0.5) * Z_NEAR;
  float bottom = -top;
  float left = aspect * bottom;
  float right = aspect * top;
  frustum(projection_matrix, left, right, bottom, top, Z_NEAR, Z_FAR);

  GLint projection_matrix_location =
      glGetUniformLocation(program->program_id, "projection_matrix");
  glUniformMatrix4fv(projection_matrix_location, 1, GL_TRUE, projection_matrix);
  // std::cout << "projection_matrix_location " << projection_matrix_location <<
  // std::endl;
}
