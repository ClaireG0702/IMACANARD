#pragma once

// GLFW et GLBASIMAC
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/matrix_stack.hpp"

#include "player.hpp"
#include "map.hpp"

#include <iostream>
#include <cmath>
#include <vector>

using namespace glbasimac;

// interface en OpenGL
// interaction (quit)
// menu
// ecran de fin de jeu
// si on a le time : il peut choisir la taille de la grille et son skin

extern GLBI_Engine myEngine;

int ABCD();

void initScene();
void renderScene();