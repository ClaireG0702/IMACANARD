#pragma once
#include "includes/map.hpp"

// mouvement

// manger les ombres
// minage (détruire un bloc)

struct Player {
    glm::vec2 position {};
    Direction direction {};
    STP3D::StandardMesh *square;

    void digging (Cell& block);
};

void initPlayer(Player &player, std::vector<Cell> &map);
