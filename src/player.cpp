#include "includes/player.hpp"
#include "includes/map.hpp"

void Player::digging (Cell& block){ //action miner
    std::srand(std::time(nullptr));
    int prob_factor{std::rand()%6};

    if (block.value == 1){
        if (prob_factor == 5){ // 1 chance sur 5 de trouver un objet dans le bloc destructible.
            block.value = 2;
        } else{
            block.value = 0;
        }
    }
}

void initPlayer(Player &player, std::vector<Cell> &map) {
    if (map.empty()) return;

    // Trouver les dimensions de la carte
    float maxX = 0, maxY = 0;
    for (const Cell& cell : map) {
        if (cell.positions.x > maxX) maxX = cell.positions.x;
        if (cell.positions.y > maxY) maxY = cell.positions.y;
    }

    float centerX = maxX / 2.0f;
    float centerY = maxY / 2.0f;

    // Trouver la cellule la plus proche du centre avec value == 0
    Cell* closestCell = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (Cell& cell : map) {
        if (cell.value != 0) continue;

        float dx = cell.positions.x - centerX;
        float dy = cell.positions.y - centerY;
        float distance = dx * dx + dy * dy; // pas besoin de racine carrée ici

        if (distance < minDistance) {
            minDistance = distance;
            closestCell = &cell;
        }
    }

    // Fallback si aucune cellule valide (devrait jamais arriver si carte bien générée)
    if (!closestCell) {
        int index = std::rand() % map.size();
        player.position = map[index].positions;
        player.gridPos = map[index].positions;
    } else {
        player.position = closestCell->positions;
        player.gridPos = closestCell->positions;
    }

    player.direction = Direction::NONE;
}