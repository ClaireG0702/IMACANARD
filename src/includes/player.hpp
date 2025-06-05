#pragma once
#include "includes/map.hpp"

struct Player
{
    glm::vec2 position{};
    glm::vec2 gridPos{};
    Direction direction{};
    float speed{2.0f};
    int objectsNbr{0};

    void digging(Cell &block);
};

void initPlayer(Player &player, std::vector<Cell> &map);
