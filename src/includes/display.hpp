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

#include "player.hpp"
#include "character.hpp"
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

void initScene();
void renderScene();

StandardMesh *createCellBuffer(float const x, float const y, float cellWidth, float cellHeight);
std::vector<GLBI_Texture> initTextures();
GLBI_Texture createOneTexture(const char *filename);
void setTypeCell(Cell const &cell);
void initCell(Cell &cell, float const x, float const y, float cellWidth, float cellHeight);
void initMap(std::vector<Cell> &map);
void drawBaseMap(std::vector<Cell> const &map);
GLBI_Texture setTextureCell(Cell const &cell, std::vector<GLBI_Texture> const &allTextures);

void initAllCharacters(Player &player, std::vector<Cell> &map);
void drawAllCharacters(Player &player);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void drawTexturedBaseMap(std::vector<Cell> const &map, std::vector<GLBI_Texture> const &allTextures);
