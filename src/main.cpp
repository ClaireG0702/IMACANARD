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

static std::vector<Cell> map{};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS ||action == GLFW_REPEAT)){
        player.direction = Direction::UP;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS ||action == GLFW_REPEAT)){
        player.direction = Direction::DOWN;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS ||action == GLFW_REPEAT)){
        player.direction = Direction::LEFT;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS ||action == GLFW_REPEAT)){
        player.direction = Direction::RIGHT;
    }
    
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS ||action == GLFW_REPEAT)){ // Quand on appuie sur 'A', on mine le bloc devant nous
        auto x {player.position.x};
        auto y {player.position.y};
        
        switch (player.direction)
        {
        case Direction::UP :{
            y -= 1;

            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                    [x,y](const Cell &cell) {return (cell.positions.x == x && cell.positions.y == y);}
                )
            };
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::RIGHT :{
            x += 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                    [x,y](const Cell &cell) {return (cell.positions.x == x && cell.positions.y == y);}
                )
            };
            player.digging(*playerNeighbor);
            break;
        }

        case Direction::DOWN :{
            y += 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                    [x,y](const Cell &cell) {return (cell.positions.x == x && cell.positions.y == y);}
                )
            };
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::LEFT :{
            x -= 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                    [x,y](const Cell &cell) {return (cell.positions.x == x && cell.positions.y == y);}
                )
            };
            player.digging(*playerNeighbor);

            break;
        }
        default:{
            break;}
        }
	}
}

int main()
{
    ABCD();
    // std::vector<std::vector<unsigned int>> oldMap{generateMap(10, 10)};
    map = generateMap(10, 10);
    displayMap(map);
    std::cout << std::endl;

    std::vector<Cell> newMap{};
    newMap = generateCellularMap(map, 4);

    displayMap(newMap);
    return 0;
}