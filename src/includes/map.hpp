#pragma once

// for map
#include <vector>

// for map display
#include <iostream>

// for random cases
#include <cstdlib>
#include <ctime>

// génération map
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

void displayMap(std::vector<std::vector<unsigned int>> const &map);
std::vector<std::vector<unsigned int>> generateMap(size_t const height, size_t const width);