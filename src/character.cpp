#include "includes/character.hpp"

void updatePlayerPosition(std::vector<Cell> &map, int cellSize, Player &player) {
    switch (player.direction) {
        case Direction::UP:
            player.position.y += 0.5;
            break;
        case Direction::RIGHT:
            player.position.x += 0.5;
            break;
        case Direction::DOWN:
            player.position.y -= 0.5;
            break;
        case Direction::LEFT:
            player.position.x -= 0.5;
            break;
        default:
            break;
    }
}