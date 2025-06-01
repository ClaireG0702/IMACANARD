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
        {std::floor(playerPos.x - CHARACTERSSIZE), std::floor(playerPos.y - CHARACTERSSIZE)}, // bas gauche
        {std::round(playerPos.x), std::floor(playerPos.y - CHARACTERSSIZE)}, // bas droite
        {std::floor(playerPos.x - CHARACTERSSIZE), std::round(playerPos.y)}, // haut gauche
        {std::round(playerPos.x), std::round(playerPos.y)}  // haut droite
    };

    for (const glm::vec2& corner : corners) {
        int cx = static_cast<int>(corner.x);
        int cy = static_cast<int>(corner.y);

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
        default: return;
    }

    glm::vec2 nextPos = player.position + movement * player.speed * deltaTime;

    if (checkIfPositionIsValid(map, nextPos)) {
        player.position = nextPos;
        player.gridPos = glm::vec2(
            std::floor(player.position.x),
            std::floor(player.position.y)
        );
    }
}
