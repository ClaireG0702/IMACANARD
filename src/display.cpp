#include "includes/display.hpp"

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;
static const float GL_VIEW_SIZE = 1.0;

/* OpenGL Engine */
GLBI_Engine myEngine;

#define WIDTH 25
std::vector<Cell> map; // the map to be displayed
#define CELLSIZE 1 / map.back().positions.x

GLBI_Convex_2D_Shape square; // the square to be placed
// StandardMesh *testSquare;
Player player{};

void initScene()
{
    map = generateMap(WIDTH, WIDTH);
    map = generateCellularMap(map, 4);

    initMap(map);
    initAllCharacters(player, map);
    // displayBasicMap(map);

    // TODO : define positions of obstacles
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

// create a standard mesh pointer to add coordinates of the cell based on it's size and placement
// TODO : delete the pointer somewhere
StandardMesh *createCellBuffer(float const x, float const y, float cellWidth, float cellHeight, float z)
{
    StandardMesh *cellPointer = new StandardMesh(4, GL_TRIANGLE_FAN);
    std::vector<float> squareCoords{
        x, y, z,                          // Bottom Left
        x + cellWidth, y, z,              // Bottom Right
        x + cellWidth, y + cellHeight, z, // Top Right
        x, y + cellHeight, z              // Top Left
    };

    cellPointer->addOneBuffer(0, 3, squareCoords.data(), "coordinates", true);
    return cellPointer;
}

//
void setTypeCell(Cell const &cell)
{
    switch (cell.value)
    {
    case 0:
        myEngine.setFlatColor(0, 1, 0); // Green
        break;
    case 1:
        myEngine.setFlatColor(0, 0, 1); // Blue
        break;
    default:
        myEngine.setFlatColor(0, 0, 0); // Default
        break;
    }
}

void initCell(Cell &cell, float const x, float const y, float cellWidth, float cellHeight)
{
    cell.square = createCellBuffer(x, y, cellWidth, cellHeight, 0.f); // the cell's square have a buffer with it's coordinates
    cell.square->createVAO();
};

void initMap(std::vector<Cell> &map)
{
    // int width, height;
    // glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    float rows{map.back().positions.y}; // number of rows same as last y ↕
    float cols{map.back().positions.x}; // number of cols same as last x ↔

    float cellWidth = 1 / cols;
    float cellHeight = 1 / rows;

    for (Cell &cell : map)
    {
        int col{static_cast<int>(cell.positions.x)}; // which row we are on ↕
        int row{static_cast<int>(cell.positions.y)}; // which col we are on ↔

        float x{-1 / 2.0f + col * cellWidth};  // to transform it to -1/1 coordinates
        float y{-1 / 2.0f + row * cellHeight}; // to transform it to -1/1 coordinates

        initCell(cell, x, y, cellWidth, cellHeight); // to initialize a cell
    }
};

void initAllCharacters(Player &player, std::vector<Cell> &map)
{
    // Initialize player position
    initPlayer(player, map);

    float cellSize = 1 / map.back().positions.x; // size of a cell based on the number of columns
    float x = -1 / 2.0f + player.position.x * cellSize;
    float y = -1 / 2.0f + player.position.y * cellSize;

    player.square = createCellBuffer(x, y, cellSize/2, cellSize/2, 1.0f);
    player.square->createVAO();
}

void updatePlayerMesh(Player &player, float cellSize)
{
    float x = -0.5f + player.position.x * cellSize;
    float y = -0.5f + player.position.y * cellSize;

    // Supprimer l'ancien mesh
    if (player.square != nullptr)
        delete player.square;

    player.square = createCellBuffer(x, y, cellSize / 2.0f, cellSize / 2.0f, 1.0f);
    player.square->createVAO();
}


void drawBaseMap(std::vector<Cell> const &map)
{
    for (Cell const &cell : map)
    {
        setTypeCell(cell);
        cell.square->draw();
    };
}

void drawAllCharacters(Player &player)
{
    myEngine.setFlatColor(1, 0, 0); // Red
    player.square->draw();
}

bool mapGenerated{false};

void renderScene()
{
    drawBaseMap(map);
    drawAllCharacters(player);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::UP;
        updatePlayerPosition(map, CELLSIZE, player);
        updatePlayerMesh(player, CELLSIZE);
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::DOWN;
        updatePlayerPosition(map, CELLSIZE, player);
        updatePlayerMesh(player, CELLSIZE);
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::LEFT;
        updatePlayerPosition(map, CELLSIZE, player);
        updatePlayerMesh(player, CELLSIZE);
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::RIGHT;
        updatePlayerPosition(map, CELLSIZE, player);
        updatePlayerMesh(player, CELLSIZE);
    }

    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
    { // Quand on appuie sur 'A', on mine le bloc devant nous
        auto x{player.position.x};
        auto y{player.position.y};

        switch (player.direction)
        {
        case Direction::UP:
        {
            y -= 1;

            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
                { return (cell.positions.x == x && cell.positions.y == y); }
            )};
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::RIGHT:
        {
            x += 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
                { return (cell.positions.x == x && cell.positions.y == y); }
            )};
            player.digging(*playerNeighbor);
            break;
        }

        case Direction::DOWN:
        {
            y += 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
                { return (cell.positions.x == x && cell.positions.y == y); }
            )};
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::LEFT:
        {
            x -= 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
                { return (cell.positions.x == x && cell.positions.y == y); }
            )};
            player.digging(*playerNeighbor);

            break;
        }
        default:
        {
            break;
        }
        }
    }
}