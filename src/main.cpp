#include "includes/character.hpp"
#include "includes/display.hpp"
#include "includes/enemy.hpp"
#include "includes/map.hpp"
#include "includes/player.hpp"

// GLM
#include "glm/fwd.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

// GLFW et GLBASIMAC
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"

#include <algorithm>
#include <iterator>

#define WIDTH 10

static Player player{};

// void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_Q && action == GLFW_PRESS)
//     {
//         Block blockInFront{}; // bloc qu'on veut détruire
//         switch (player.direction)
//         {
//         case Direction::UP:
//             blockInFront = map[];
//             break;

//         case Direction::RIGHT:
//             /* code */
//             break;

//         case Direction::DOWN:
//             /* code */
//             break;

//         case Direction::LEFT:
//             /* code */
//             break;

//         default:
//             break;
//         }

//         player.digging(blockInFront);
//     }
// }

int main()
{
    ABCD();
    // std::vector<std::vector<unsigned int>> oldMap{generateMap(10, 10)};
    std::vector<Cell> map{generateMap(10, 10)};
    displayMap(map);
    std::cout << std::endl;
    cellularAutomata(map);

    displayMap(map);
    std::cout << std::endl;

    std::vector<Cell> newMap{};
    newMap = cellularAutomata(map);

    displayMap(newMap);
    return 0;
}