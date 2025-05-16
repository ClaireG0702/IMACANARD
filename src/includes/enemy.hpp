#pragma once

#include "map.hpp"
#include "player.hpp"

#include <algorithm>
#include <iterator>

// algorithme de recherche
// déplacement -> (presque) même que le joueur

std::vector<Cell> createValuedMap(std::vector<Cell> &map, Player &posPlayer);

std::vector<CellDirection> createDirectedMap(std::vector<Cell> &map);