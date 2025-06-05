#include "includes/character.hpp"

bool isCellFree(const std::vector<Cell>& map, int x, int y) {
    for (const Cell& cell : map) {
        if ((int)cell.positions.x == x && (int)cell.positions.y == y) {
            return (cell.value != 1 && cell.value != 2);
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

        if (cx < 0 || cy < 0 || cx >= width || cy >= width || !isCellFree(map, cx, cy)) {
            return false;
        }
    }

    return true;
}

bool checkElements(std::vector<Cell>& map, Player& player) {
    glm::vec2 corners[4] = {
        {player.position.x - 0.5f, player.position.y - 0.5f}, // bas gauche
        {player.position.x, player.position.y - 0.5f}, // bas droite
        {player.position.x - 0.5f, player.position.y}, // haut gauche
        {player.position.x, player.position.y}  // haut droite
    };

    for (const glm::vec2& corner : corners) {
        int cx = std::round(corner.x);
        int cy = std::round(corner.y);

        auto mapIt = std::find_if(map.begin(), map.end(), [cx, cy](const Cell &cell) {
            return cell.positions.x == cx && cell.positions.y == cy;
        });

        if (cx >= 0 && cy >= 0 && cx < width && cy < width && (mapIt->value == 5 || mapIt->value == 4)) {
            if(mapIt->value == 5) {
                gameStatus = GameStatus::LOSE;
                currentPage = Page::END_SCREEN;
            }
            if(mapIt->value == 4) {
                player.objectsNbr++;
                remainingObjects--;
                mapIt->value = 0;

                if(player.objectsNbr == numberOfObjects) {
                    gameStatus = GameStatus::WIN;
                    currentPage = Page::END_SCREEN;
                }
            }
            
            return true;
        }
    }

    return false;
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

    auto x = player.position.x;
    auto y = player.position.y;

    bool isBooster {isCellBoost(map, x, y)};
    if (isBooster){
        player.speed += 2;
    }

    bool isSlow {isCellSlow(map, x, y)};
    if (isSlow && player.speed>1.0f){
        player.speed -= 0.5f;
    }

    glm::vec2 nextPos = player.position + movement * player.speed * deltaTime;
    
    if(player.speed > 2.0f){
        player.speed -= 1.0f;
    }

    if(player.speed < 2.0f){
        player.speed += 0.25f;
    }

    if (checkIfPositionIsValid(map, nextPos)) {
        player.position = nextPos;
        player.gridPos = glm::vec2(
            std::round(player.position.x),
            std::round(player.position.y)
        );
        checkElements(map, player);
    }
}

bool isCellBoost(const std::vector<Cell>& map, int x, int y) {
    for (const Cell& cell : map) {
        if ((int)cell.positions.x == x && (int)cell.positions.y == y) {
            return cell.value == 3;
        }
    }
    return false;
}

bool isCellSlow(const std::vector<Cell>& map, int x, int y) {
    for (const Cell& cell : map) {
        if ((int)cell.positions.x == x && (int)cell.positions.y == y) {
            return cell.value == 6;
        }
    }
    return false;
}