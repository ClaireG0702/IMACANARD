#pragma once

// génération map
// placement objets
// placement aléatoire
// gestion des blocs

enum class BlockType
{
    EMPTY,    // water
    PLAIN,    // bloc qu'on peut détruire --> nénuphar
    TRAP,     // piège
    OBSTACLE, // wood, rock
    OBJECT,   // shadow + anim si on a le temps
};

// enum class Actions
// {
//     MINE,
//     SPEED,
//     SLOW,
// }

struct Block
{
    BlockType type{};
    // bool object{};
};