#include "includes/player.hpp"
#include "includes/map.hpp"

void Player::digging (Block* block){ //action miner
    std::srand(std::time(nullptr));
    int prob_factor{std::rand()%6};
    // 1 chance sur 5 de trouver un objet dans le bloc destructible.
    if (block->type == BlockType::PLAIN){
        if (prob_factor == 5){
            block->type = BlockType::OBJECT;
        } else{
            block->type = BlockType::EMPTY;
        }
    }
}