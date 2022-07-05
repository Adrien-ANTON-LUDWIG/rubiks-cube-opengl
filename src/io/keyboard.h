#pragma once

#include "../rubiks_cube/rubiks_cube.h"
#include "../utils/ascii_table.h"

void keyboard_normal_callback(unsigned char key, int x, int y) {
  x = x;  // Avoid unused variable warning
  y = y;  // Avoid unused variable warning

  switch (key) {
    // Quit
    case ASCII_ESC:
      exit(0);

    // Mouvements de base //

    // Right
    case ASCII_R_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, AXIS_X);
      glutPostRedisplay();
      break;
    case ASCII_R_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, AXIS_X);
      glutPostRedisplay();
      break;
    // Left
    case ASCII_L_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, -AXIS_X);
      glutPostRedisplay();
      break;
    case ASCII_L_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, -AXIS_X);
      glutPostRedisplay();
      break;
    // Up
    case ASCII_U_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, AXIS_Y);
      glutPostRedisplay();
      break;
    case ASCII_U_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, AXIS_Y);
      glutPostRedisplay();
      break;
    // Down
    case ASCII_D_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, -AXIS_Y);
      glutPostRedisplay();
      break;
    case ASCII_D_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, -AXIS_Y);
      glutPostRedisplay();
      break;
    // Front
    case ASCII_F_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, AXIS_Z);
      glutPostRedisplay();
      break;
    case ASCII_F_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, AXIS_Z);
      glutPostRedisplay();
      break;
    // Down
    case ASCII_B_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, -AXIS_Z);
      glutPostRedisplay();
      break;
    case ASCII_B_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, -AXIS_Z);
      glutPostRedisplay();
      break;
  }
}