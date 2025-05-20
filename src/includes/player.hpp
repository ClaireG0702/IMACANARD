#pragma once
#include "includes/map.hpp"

// mouvement

// manger les ombres
// minage (détruire un bloc)

struct Player {
    glm::vec2 position {};
    Direction direction {};

    void digging (Cell& block);
};

