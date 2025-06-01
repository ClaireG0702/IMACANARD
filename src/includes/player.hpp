#pragma once
#include "includes/map.hpp"

// mouvement

// manger les ombres
// minage (détruire un bloc)

struct Player {
    glm::vec2 position {};
    glm::vec2 gridPos {};
    Direction direction {};
    float speed {2.0f};

    void digging (Cell& block);
};

void initPlayer(Player &player, std::vector<Cell> &map);
