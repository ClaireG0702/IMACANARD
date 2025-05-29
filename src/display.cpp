#include "includes/display.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;
static const float GL_VIEW_SIZE = 1.0;

/* OpenGL Engine */
GLBI_Engine myEngine;

#define WIDTH 25
std::vector<Cell> map; // the map to be displayed
#define CELLSIZE 1 / map.back().positions.x

Player player{};
std::vector<Enemy> enemies;
std::vector<CellDirection> directedMap;

std::vector<GLBI_Texture> allTextures{};

void initScene()
{
    map = generateMap(WIDTH, WIDTH);
    map = generateCellularMap(map, 4);

    allTextures = initTextures();
    initMap(map);
    initAllCharacters(player, map);
    // displayBasicMap(map);

    // TODO : define positions of obstacles
    // TODO : define positions of player (center of the map)
    // TODO : define positions of enemies
}

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

GLBI_Texture createOneTexture(const char *filename)
{
    int x{};
    int y{};
    int n{};

    unsigned char *pixels{stbi_load(filename, &x, &y, &n, 0)};
    std::cout << "Image " << filename << (pixels != nullptr ? "" : " not") << " loaded." << std::endl;

    GLBI_Texture texture{};
    texture.createTexture();
    texture.attachTexture();
    texture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture.loadImage(x, y, n, pixels);
    texture.detachTexture();
    stbi_image_free(pixels);

    return texture;
};

std::vector<GLBI_Texture> initTextures()
{
    std::vector<GLBI_Texture> allTextures{};

    std::array<char const *, 4> filenames{"assets/images/water.png",
                                          "assets/images/yellow_ducky.png",
                                          "assets/images/brown_ducky.png",
                                          "assets/images/error.png"};

    for (char const *filename : filenames)
    {
        GLBI_Texture texture{};
        texture = createOneTexture(filename);

        allTextures.push_back(texture);
    }

    return allTextures;
}

GLBI_Texture setTextureCell(Cell const &cell, std::vector<GLBI_Texture> const &allTextures)
{
    GLBI_Texture cellTexture{};

    switch (cell.value)
    {
    // TODO : find out how to use specific sprites
    case 0:
        cellTexture = allTextures[0];
        break;
    case 1:
        cellTexture = allTextures[1];
        break;
    case 2:
        cellTexture = allTextures[2];
        break;

    default:
        cellTexture = allTextures[3]; // error
        break;
    }

    return cellTexture;
};

void setTypeCell(Cell const &cell)
{
    switch (cell.value)
    {
    case 0:
        myEngine.setFlatColor(0, 0, 1); // Green
        break;
    case 1:
        myEngine.setFlatColor(0, 1, 0); // Blue
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

    /*enemies = std::vector<Enemy>(2);
    initEnemies(enemies, map);
    for(Enemy& enemy: enemies) {
        float ex = -0.5f + enemy.position.x * cellSize;
        float ey = -0.5f + enemy.position.y * cellSize;
        enemy.square = createCellBuffer(ex, ey, cellSize / 2, cellSize / 2, 1.0f);
        enemy.square->createVAO();
    }*/
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

void updateEnemiesMesh(std::vector<Enemy> &enemies, float cellSize)
{
    for (Enemy &enemy : enemies)
    {
        float x = -0.5f + enemy.position.x * cellSize;
        float y = -0.5f + enemy.position.y * cellSize;

        if (enemy.square != nullptr)
            delete enemy.square;

        enemy.square = createCellBuffer(x, y, cellSize / 2.0f, cellSize / 2.0f, 1.0f);
        enemy.square->createVAO();
    }
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

    myEngine.setFlatColor(1,1,0);
    for(Enemy& enemy: enemies) {
        enemy.square->draw();
    }
}

void drawTexturedBaseMap(std::vector<Cell> const &map, std::vector<GLBI_Texture> const &allTextures)
{
    for (Cell const &cell : map)
    {
        GLBI_Texture cellTexture{setTextureCell(cell, allTextures)}; // change based on cell.value
        cellTexture.attachTexture();
        cell.square->draw();
        cellTexture.detachTexture();
    };
}

void renderScene()
{
    //drawTexturedBaseMap(map, allTextures);
    drawBaseMap(map);
    drawAllCharacters(player);

    updateEnemies(enemies, directedMap);
    updateEnemiesMesh(enemies, CELLSIZE);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    bool isPositionUpdated = false;
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::UP;
        isPositionUpdated = updatePlayerPosition(map, CELLSIZE, player);
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::DOWN;
        isPositionUpdated = updatePlayerPosition(map, CELLSIZE, player);
    }
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::LEFT;
        isPositionUpdated = updatePlayerPosition(map, CELLSIZE, player);
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        player.direction = Direction::RIGHT;
        isPositionUpdated = updatePlayerPosition(map, CELLSIZE, player);
    }

    if(isPositionUpdated) {
        updatePlayerMesh(player, CELLSIZE);

        auto valuedMap = createValuedMap(map, player);
        directedMap = createDirectedMap(valuedMap);
    }

    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
    { // Quand on appuie sur 'A', on mine le bloc devant nous
        auto x{player.position.x};
        auto y{player.position.y};

        switch (player.direction)
        {
        case Direction::UP:
        {
            y += 1;

            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
                { return (cell.positions.x == x && cell.positions.y == y); })};
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::RIGHT:
        {
            x += 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
                { return (cell.positions.x == x && cell.positions.y == y); })};
            player.digging(*playerNeighbor);
            break;
        }

        case Direction::DOWN:
        {
            y -= 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
                { return (cell.positions.x == x && cell.positions.y == y); })};
            player.digging(*playerNeighbor);

            break;
        }
        case Direction::LEFT:
        {
            x -= 1;
            auto playerNeighbor{std::find_if(map.begin(), map.end(), [x, y](const Cell &cell)
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