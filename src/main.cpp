#include "includes/character.hpp"
#include "includes/display.hpp"
#include "includes/enemy.hpp"
#include "includes/map.hpp"
#include "includes/player.hpp"

int main()
{
    std::vector<std::vector<unsigned int>> map{generateMap(10, 10)};
    displayMap(map);

    return 0;
}