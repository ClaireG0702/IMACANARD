#pragma once

// for map
#include <vector>
#include <algorithm>
#include <optional>
#include <array>
#include <numeric>

// for blocks
#include "tools/basic_mesh.hpp"

// for map display
#include <iostream>

// for random cases
#include <cstdlib>
#include <ctime>

// map generation
#include "glm/fwd.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

#include "game_state.hpp"

// placement objets
// placement aléatoire
// gestion des blocs


enum class Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};

// enum class Actions
// {
//     SPEED,
//     SLOW,
// }


struct Cell
{
    glm::vec2 positions; // x,y
    unsigned int value;
};

struct CellDirection
{
    glm::vec2 positions; // x,y
    Direction direction;
};

void displayMap(std::vector<Cell> const &map);
std::vector<Cell> generateMap(size_t const height, size_t const width);
std::optional<Cell> findNeighbour(std::vector<Cell> const &map, Cell const &cell, int movementX, int movementY);
std::vector<Cell> adjacentCells(std::vector<Cell> const &map, Cell const &cell); // return vect of adjacent cells

std::vector<Cell> cellularAutomata(std::vector<Cell> const &map);
std::vector<Cell> generateCellularMap(std::vector<Cell> const &map, int nbIterations);
void displayValuedMap(std::vector<Cell> const &map);
void displayDirectedMap(std::vector<CellDirection> const &map);

void addObjectsAndTraps(std::vector<Cell> &map);
void addMinableBlocs(std::vector<Cell> &map);
void addBonusBlocks(std::vector<Cell> &map);