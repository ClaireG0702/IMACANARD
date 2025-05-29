#include "includes/character.hpp"

bool updatePlayerPosition(std::vector<Cell> &map, int cellSize, Player &player) {
    int x = player.position.x, y = player.position.y;
    bool isPlayerPositionUpdated = false;
    
    switch (player.direction) {
        case Direction::UP:
            y += 1;
            break;
        case Direction::RIGHT:
            x += 1;
            break;
        case Direction::DOWN:
            y -= 1;
            break;
        case Direction::LEFT:
            x -= 1;
            break;
        default:
            break;
    }

    if(checkIfPositionIsValid(map, cellSize, x, y)) {
        player.position.x = x;
        player.position.y = y;
        isPlayerPositionUpdated = true;
    }

    return isPlayerPositionUpdated;
}

bool checkIfPositionIsValid(std::vector<Cell> &map, int cellSize, int x, int y) {
    // Vérifie que la position est dans la map
    if (x < map.front().positions.x || y < map.front().positions.y || x >= map.back().positions.x || y >= map.back().positions.y)
        return false;

    // Recherche la cellule correspondante
    for (const Cell& cell : map) {
        if (static_cast<int>(cell.positions.x) == x && static_cast<int>(cell.positions.y) == y) {
            // Vérifie que la valeur n'est pas 1
            return cell.value != 1;
        }
    }
    // Si aucune cellule trouvée à cette position, on considère que ce n'est pas valide
    return false;
}