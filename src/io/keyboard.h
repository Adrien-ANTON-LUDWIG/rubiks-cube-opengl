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

    // Reset view angle
    case ASCII_SPACE:
      angle_alpha = INITIAL_ANGLE_ALPHA;
      angle_beta = INITIAL_ANGLE_BETA;
      update_position();
      break;

    // Reset rubik's cube original state
    case ASCII_CR:
      rubiks_cube = RubiksCube();
      break;

    // Mouvements de base //

    // Right
    case ASCII_R_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, AXIS_X);
      break;
    case ASCII_R_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, AXIS_X);
      break;
    // Left
    case ASCII_L_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, -AXIS_X);
      break;
    case ASCII_L_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, -AXIS_X);
      break;
    // Up
    case ASCII_U_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, AXIS_Y);
      break;
    case ASCII_U_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, AXIS_Y);
      break;
    // Down
    case ASCII_D_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, -AXIS_Y);
      break;
    case ASCII_D_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, -AXIS_Y);
      break;
    // Front
    case ASCII_F_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, AXIS_Z);
      break;
    case ASCII_F_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, AXIS_Z);
      break;
    // Down
    case ASCII_B_LOWER:
      rubiks_cube.rotate_face(ANGLE_CLOCKWISE, -AXIS_Z);
      break;
    case ASCII_B_UPPER:
      rubiks_cube.rotate_face(ANGLE_COUNTER_CLOCKWISE, -AXIS_Z);
      break;
  }

  glutPostRedisplay();
}