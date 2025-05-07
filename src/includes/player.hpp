#pragma once
#include "includes/map.hpp"

// mouvement

// manger les ombres
// minage (détruire un bloc)

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Player {
    std::vector<unsigned int> position {};
    Direction direction {};

    void digging (Block* block);
};

