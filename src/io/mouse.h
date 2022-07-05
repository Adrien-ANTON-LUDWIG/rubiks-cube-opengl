#pragma once

#include "camera.h"

void mouse_motion_callback(int x, int y) {
  double alpha = angle_alpha - (old_pos_x - x) * PI / 500.0;
  double beta = angle_beta - (old_pos_y - y) * PI / 500.0;

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
