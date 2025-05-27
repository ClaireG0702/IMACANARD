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
// StandardMesh *testSquare;
Player player{};

void initScene()
{
    map = generateMap(WIDTH, WIDTH);
    map = generateCellularMap(map, 4);

    initAllSquares(map);
    // displayBasicMap(map);

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

void drawCell(float const value, float const x, float const y, float cellWidth, float cellHeight)
{
    std::vector<float> squareCoords{
        x, y,                          // Bottom Left
        x + cellWidth, y,              // Bottom Right
        x + cellWidth, y + cellHeight, // Top Right
        x, y + cellHeight              // Top Left
    };

    square.initShape(squareCoords);
    if (value == 0)
    {
        myEngine.setFlatColor(0, 1, 0); // Green
    }
    else
    {
        myEngine.setFlatColor(0, 0, 1); // Blue
    }
    square.changeNature(GL_TRIANGLE_FAN);
    square.drawShape();
};
void displayBasicMap(std::vector<Cell> const &map)
{
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    float rows{map.back().positions.y}; // number of rows same as last y ↕
    float cols{map.back().positions.x}; // number of cols same as last x ↔

    float worldWidth = 1.0;
    float worldHeight = 1.0;

    float cellWidth = worldWidth / cols;
    float cellHeight = worldHeight / rows;

    for (Cell const &cell : map)
    {
        std::cout << "row " << cell.positions.y << ", col " << cell.positions.x << std::endl;
        std::cout << "value " << cell.value << std::endl;
        int row{static_cast<int>(cell.positions.y)}; // which row we are on ↕
        int col{static_cast<int>(cell.positions.x)}; // which col we are on ↔

        float x{-worldWidth / 2.0f + col * cellWidth};
        float y{-worldHeight / 2.0f + row * cellHeight};
        std::cout << "new x " << x << ", new y " << y << std::endl;
        std::cout << "cellWidth " << cellWidth << ", cellHeight " << cellHeight << std::endl;

        drawCell(cell.value, x, y, cellWidth, cellHeight);
    }
}

StandardMesh *createSMsquare(float const x, float const y, float cellWidth, float cellHeight)
{
    StandardMesh *squareSM = new StandardMesh(4, GL_TRIANGLE_STRIP);
    std::vector<float> squareCoords{
        x, y, 0,                          // Bottom Left
        x + cellWidth, y, 0,              // Bottom Right
        x + cellWidth, y + cellHeight, 0, // Top Right
        x, y + cellHeight, 0              // Top Left
    };

    squareSM->addOneBuffer(0, 3, squareCoords.data(), "coordinates", true);
    return squareSM;
}

void drawTest(Cell const &cell)
{
    if (cell.value == 0)
    {
        myEngine.setFlatColor(0, 1, 0); // Green
    }
    else
    {
        myEngine.setFlatColor(0, 0, 1); // Blue
    }
}

void initTest(float const x, float const y, float cellWidth, float cellHeight, Cell &cell)
{
    cell.square = createSMsquare(x, y, cellWidth, cellHeight);
    cell.square->createVAO();
};

void initAllSquares(std::vector<Cell> &map)
{
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    float rows{map.back().positions.y}; // number of rows same as last y ↕
    float cols{map.back().positions.x}; // number of cols same as last x ↔

    float cellWidth = 1 / cols;
    float cellHeight = 1 / rows;

    for (Cell &cell : map)
    {
        int col{static_cast<int>(cell.positions.x)}; // which row we are on ↕
        int row{static_cast<int>(cell.positions.y)}; // which col we are on ↔

        float x{-1 / 2.0f + col * cellWidth};
        float y{-1 / 2.0f + row * cellHeight};

        initTest(x, y, cellWidth, cellHeight, cell);
    }
};

void drawInitAllSquares(std::vector<Cell> const &map)
{
    for (Cell const &cell : map)
    {
        drawTest(cell);
        cell.square->draw();
    };
}

bool mapGenerated{false};

void renderScene()
{
    // if (!mapGenerated)
    // {
    //     displayBasicMap(map);
    //     mapGenerated = true;
    // }
    drawInitAllSquares(map);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::UP;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::DOWN;
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::LEFT;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::RIGHT;
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

            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                                             [x, y](const Cell &cell)
                                             { return (cell.positions.x == x && cell.positions.y == y); })};
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::RIGHT:
        {
            x += 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                                             [x, y](const Cell &cell)
                                             { return (cell.positions.x == x && cell.positions.y == y); })};
            player.digging(*playerNeighbor);
            break;
        }

        case Direction::DOWN:
        {
            y += 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                                             [x, y](const Cell &cell)
                                             { return (cell.positions.x == x && cell.positions.y == y); })};
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::LEFT:
        {
            x -= 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(),
                                             [x, y](const Cell &cell)
                                             { return (cell.positions.x == x && cell.positions.y == y); })};
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