#include "includes/display.hpp"

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;
static const float GL_VIEW_SIZE = 1.0;

/* OpenGL Engine */
GLBI_Engine myEngine;

#define WIDTH 10
std::vector<Cell> map; // the map to be displayed

GLBI_Convex_2D_Shape square; // the square to be placed

Player player{};

void initScene()
{
    map = generateMap(WIDTH, WIDTH);

    // TODO : definie positions of obstacles
    // TODO : define positions of player (center of the map)
    // TODO : define positions of enemies

}
/*int main() ----> old main the definition here need to be done on initScene
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
}*/

void renderScene()
{
	int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	
    int rows = 10;
    int cols = 10;
	
    float worldWidth = 1.0;
    float worldHeight = 1.0;
	
    float cellWidth = worldWidth / cols;
    float cellHeight = worldHeight / rows;
	
    for (Cell const &cell : map)
    {
        std::cout << "x " << cell.positions[0] << ", y " << cell.positions[1] << std::endl;
		std::cout << "value " << cell.value << std::endl;
        int row = cell.positions[0];
        int col = cell.positions[1];

        float x = -worldWidth / 2.0f + col * cellWidth;
        float y = -worldHeight / 2.0f + row * cellHeight;

        std::vector<float> squareCoords = {
            x, y,                          // Bottom Left
            x + cellWidth, y,              // Bottom Right
            x + cellWidth, y + cellHeight, // Top Right
            x, y + cellHeight              // Top Left
        };
        
        square.initShape(squareCoords);
        if (cell.value == 0)
        {
			myEngine.setFlatColor(0, 1, 0); // Green
        }
        else
        {
			myEngine.setFlatColor(0, 0, 1); // Blue
        }
        square.changeNature(GL_TRIANGLE_FAN);
        square.drawShape();
    }
}

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