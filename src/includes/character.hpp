#pragma once

#include "game_state.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "map.hpp"

/*Gestion des collisions
- je suis dans une tuile si au moins des 4 corners est lui aussi dans la grille
- pour vérifier : prendre flottant --> arrondir au précédent (avec un round ou un float)

je calcule position possible + est ce que dans les 4 tuiles possibles y en a 1 qui est pleine ?
    - si oui : j'avance
    - si non : je bouge pas ou action
*/

void updatePlayerPosition(std::vector<Cell> &map, float deltaTime, Player &player);
bool checkIfPositionIsValid(const std::vector<Cell> &map, glm::vec2 playerPos);
bool checkElements(std::vector<Cell> &map, Player &player);
bool isCellFree(const std::vector<Cell> &map, int x, int y);
bool isCellBoost(const std::vector<Cell> &map, int x, int y);
bool isCellSlow(const std::vector<Cell> &map, int x, int y);