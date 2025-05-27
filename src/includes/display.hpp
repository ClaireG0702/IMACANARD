#pragma once

// GLFW et GLBASIMAC
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/matrix_stack.hpp"
#include "tools/basic_mesh.hpp"

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
void drawCell(float const value, float const x, float const y, float cellWidth, float cellHeight);
void displayBasicMap(std::vector<Cell> const &map);

StandardMesh *createSMsquare(float const x, float const y, float cellWidth, float cellHeight);
void initTest(float const x, float const y, float cellWidth, float cellHeight, Cell &cell);
void drawTest(Cell const &cell, STP3D::StandardMesh *testSquare);
void initAllSquares(std::vector<Cell> &map);
void drawInitAllSquares(std::vector<Cell> const &map);
void initAllCharacters(Player &player, std::vector<Cell> &map);
void drawAllCharacters(Player &player);