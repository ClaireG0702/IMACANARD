#include "includes/character.hpp"

void updatePlayerPosition(std::vector<Cell> &map, int cellSize, Player &player) {
    int x = player.position.x;
    int y = player.position.y;

    switch (player.direction) {
        case Direction::UP:
            y += 0.1;
            break;
        case Direction::RIGHT:
            x += 0.1;
            break;
        case Direction::DOWN:
            y -= 0.1;
            break;
        case Direction::LEFT:
            x -= 0.1;
            break;
        default:
            break;
    }
}