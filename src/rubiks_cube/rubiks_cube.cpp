#include "rubiks_cube.h"

#include <iostream>

void RubiksCube::translate(GLfloat x, GLfloat y, GLfloat z) {
    std::cout << "Translating";
    for (auto cube : cubes) cube.translate(x, y, z);
}