#pragma once

#include "game_state.hpp"
#include "map.hpp"
#include "character.hpp"
#include "player.hpp"

#include <algorithm>
#include <iterator>

struct Enemy
{
    glm::vec2 position{};
    glm::vec2 gridPos{};
    Direction direction{};
    float speed{1.5f};
};

void initEnemies(std::vector<Enemy> &enemies, Player player, std::vector<Cell> &map);
void updateEnemies(std::vector<Enemy> &enemies, const std::vector<CellDirection> &directedMap, std::vector<Cell> &map, Player player, float deltaTime);

std::vector<Cell> createValuedMap(std::vector<Cell> &map, Player &posPlayer);
std::vector<CellDirection> createDirectedMap(std::vector<Cell> &map);