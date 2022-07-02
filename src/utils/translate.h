#pragma once

#include <vector>
#include <GL/freeglut.h>

/**
 * @brief Translate an object represented by a vecter of vertices.
 * 
 * @param object
 * @param x 
 * @param y 
 * @param z 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> translate(std::vector<GLfloat> object, float x, float y, float z);
