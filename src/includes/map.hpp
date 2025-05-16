#pragma once

// for map
#include <vector>

// for map display
#include <iostream>

// for random cases
#include <cstdlib>
#include <ctime>

// génération map
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


enum class Direction {
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

struct Neighbour
{
    unsigned int x{};
    unsigned int y{};
    unsigned int value{};
};

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
void cellularAutomata(std::vector<std::vector<unsigned int>> &map);
void digging(Block *block);

void displayValuedMap(std::vector<Cell> const &map);
void displayDirectedMap(std::vector<CellDirection> const &map);