#include "includes/character.hpp"

void updatePlayerPosition(std::vector<Cell> &map, float deltaTime, Player &player) {
    glm::vec2 movement(0.f);
    
    switch (player.direction) {
        case Direction::UP:
            movement.y = 1;
            break;
        case Direction::RIGHT:
            movement.x = 1;
            break;
        case Direction::DOWN:
            movement.y = -1;
            break;
        case Direction::LEFT:
            movement.x = -1;
            break;
        default:
            break;
    }

    glm::vec2 nextPos = player.position + movement * player.speed * deltaTime;

    int nextX = static_cast<int>(nextPos.x);
    int nextY = static_cast<int>(nextPos.y);
    if(checkIfPositionIsValid(map, nextX, nextY)) {
        player.position = nextPos;
        player.gridPos = glm::vec2(nextX, nextY);
    }
}

bool checkIfPositionIsValid(std::vector<Cell> &map, int x, int y) {
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