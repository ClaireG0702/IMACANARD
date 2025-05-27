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

// placement objets
// placement aléatoire
// gestion des blocs

enum class BlockType
{
    EMPTY,    // water
    PLAIN,    // bloc qu'on peut détruire --> nénuphar
    TRAP,     // piège
    OBSTACLE, // wood, rock
    OBJECT,   // shadow + anim si on a le temps
};

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

struct Block
{
    BlockType type{};
    // bool object{};
};

// struct Neighbour
// {
//     unsigned int x{};
//     unsigned int y{};
//     unsigned int value{};
// };

struct Cell
{
    glm::vec2 positions; // x,y
    unsigned int value;
    STP3D::StandardMesh *square;
};

struct CellDirection
{
    glm::vec2 positions; // x,y
    Direction direction;
};

void displayMap(std::vector<Cell> const &map);
std::vector<Cell> generateMap(size_t const height, size_t const width);
std::vector<Cell> cellularAutomata(std::vector<Cell> const &map);
std::vector<Cell> generateCellularMap(std::vector<Cell> const &map, int nbIterations);
void displayValuedMap(std::vector<Cell> const &map);
void displayDirectedMap(std::vector<CellDirection> const &map);
void digging(Block *block);
