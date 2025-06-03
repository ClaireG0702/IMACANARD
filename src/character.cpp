#include "includes/character.hpp"

#define WIDTH 25
#define CELLSIZE (1.0f / WIDTH)
#define CHARACTERSSIZE (1.0f / (WIDTH * 2))

bool isCellFree(const std::vector<Cell>& map, int x, int y) {
    for (const Cell& cell : map) {
        if ((int)cell.positions.x == x && (int)cell.positions.y == y) {
            return cell.value == 0;
        }
    }
    return false;
}

bool checkIfPositionIsValid(const std::vector<Cell>& map, glm::vec2 playerPos) {
    glm::vec2 corners[4] = {
        {playerPos.x - 0.5f, playerPos.y - 0.5f}, // bas gauche
        {playerPos.x, playerPos.y - 0.5f}, // bas droite
        {playerPos.x - 0.5f, playerPos.y}, // haut gauche
        {playerPos.x, playerPos.y}  // haut droite
    };

    
    for (const glm::vec2& corner : corners) {
        int cx = std::round(corner.x);
        int cy = std::round(corner.y);

        if (cx < 0 || cy < 0 || cx >= WIDTH || cy >= WIDTH || !isCellFree(map, cx, cy)) {
            return false;
        }
    }

    return true;
}

void updatePlayerPosition(std::vector<Cell>& map, float deltaTime, Player& player) {
    glm::vec2 movement(0.f);

    switch (player.direction) {
        case Direction::UP:    movement.y = 1.f; break;
        case Direction::DOWN:  movement.y = -1.f; break;
        case Direction::LEFT:  movement.x = -1.f; break;
        case Direction::RIGHT: movement.x = 1.f; break;
        default: break;
    }

    glm::vec2 nextPos = player.position + movement * player.speed * deltaTime;

    if (checkIfPositionIsValid(map, nextPos)) {
        player.position = nextPos;
        player.gridPos = glm::vec2(
            std::round(player.position.x),
            std::round(player.position.y)
        );
    }
}
