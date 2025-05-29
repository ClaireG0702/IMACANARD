#pragma once

#include "map.hpp"
#include "player.hpp"

#include <algorithm>
#include <iterator>

// algorithme de recherche
// déplacement -> (presque) même que le joueur

struct Enemy {
    glm::vec2 position {};
    Direction direction {};
    STP3D::StandardMesh *square;
};

void initEnemies(std::vector<Enemy> enemies, std::vector<Cell> &map);
void updateEnemies(std::vector<Enemy>& enemies, const std::vector<CellDirection>& directedMap);

std::vector<Cell> createValuedMap(std::vector<Cell> &map, Player &posPlayer);
std::vector<CellDirection> createDirectedMap(std::vector<Cell> &map);