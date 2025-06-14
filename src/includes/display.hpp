#pragma once

// GLFW et GLBASIMAC
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_texture.hpp"

#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/matrix_stack.hpp"
#include "tools/basic_mesh.hpp"

#include "game_state.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "character.hpp"
#include "map.hpp"
#include "includes/texture.hpp"

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

void initScene();
void renderScene();

void setTypeCell(Cell const &cell);
StandardMesh *createSharedCellMesh(Sprite const &sprite, float cellWidth, float cellHeight);
void drawMap(std::vector<Cell> const &map);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void updatePlayerMesh(Player &player);
void updateEnemiesMesh(std::vector<Enemy> &enemies);

void clearScene();