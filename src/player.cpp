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