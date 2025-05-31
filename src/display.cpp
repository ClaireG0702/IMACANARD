#include "includes/display.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"
#include <set>

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;
static const float GL_VIEW_SIZE = 1.0;

/* OpenGL Engine */
GLBI_Engine myEngine;

#define WIDTH 10
std::vector<Cell> map; // the map to be displayed
#define CELLSIZE (1.0f / WIDTH)
#define CHARACTERSSIZE (1.0f / (WIDTH * 2))

std::set<int> activeKeys;

Player player{};
std::vector<Enemy> enemies;
std::vector<CellDirection> directedMap;

std::vector<GLBI_Texture> allTextures{};

GLBI_Texture myTextureTest{};

void initScene()
{
    map = generateMap(WIDTH, WIDTH);
    map = generateCellularMap(map, 4);

    myTextureTest = createOneTexture("assets/images/yellow_ducky.png");
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

    std::vector<float> uvs{
        0.0f, 0.0f, // Bottom Left
        1.0f, 0.0f, // Bottom Right
        1.0f, 1.0f, // Top Right
        0.0f, 1.0f  // Top Left
    };

    cellPointer->addOneBuffer(0, 3, squareCoords.data(), "coordinates", true);
    cellPointer->addOneBuffer(2, 2, squareCoords.data(), "uvs", true);
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

    std::array<char const *, 4> filenames{"assets/images/Water.png",
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

    float x = -1 / 2.0f + player.position.x * CELLSIZE;
    float y = -1 / 2.0f + player.position.y * CELLSIZE;

    player.square = createCellBuffer(x, y, CHARACTERSSIZE, CHARACTERSSIZE, 1.0f);
    player.square->createVAO();

    /*enemies = std::vector<Enemy>(2);
    initEnemies(enemies, map);
    for(Enemy& enemy: enemies) {
        float ex = -0.5f + enemy.position.x * CELLSIZE;
        float ey = -0.5f + enemy.position.y * CELLSIZE;
        enemy.square = createCellBuffer(ex, ey, CHARACTERSSIZE, CHARACTERSSIZE, 1.0f);
        enemy.square->createVAO();
    }*/
}

void updatePlayerMesh(Player &player)
{
    float x = -0.5f + player.position.x * CELLSIZE;
    float y = -0.5f + player.position.y * CELLSIZE;

    // Supprimer l'ancien mesh
    if (player.square != nullptr)
        delete player.square;

    player.square = createCellBuffer(x, y, CHARACTERSSIZE, CHARACTERSSIZE, 1.0f);
    player.square->createVAO();
}

void updateEnemiesMesh(std::vector<Enemy> &enemies)
{
    for (Enemy &enemy : enemies)
    {
        float x = -0.5f + enemy.position.x * CELLSIZE;
        float y = -0.5f + enemy.position.y * CELLSIZE;

        if (enemy.square != nullptr)
            delete enemy.square;

        enemy.square = createCellBuffer(x, y, CHARACTERSSIZE, CHARACTERSSIZE, 1.0f);
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

    myEngine.setFlatColor(1, 1, 0);
    for (Enemy &enemy : enemies)
    {
        enemy.square->draw();
    }
};

void drawTexturedBaseMap(std::vector<Cell> const &map, std::vector<GLBI_Texture> const &allTextures)
{
    bool test1{true};
    bool test2{true};
    for (Cell const &cell : map)
    {
        if (test1)
        {
            test1 = !test1;
            myEngine.activateTexturing(false);
            myEngine.setFlatColor(1.0, 0.0, 1.0);
            cell.square->draw();
            // myEngine.activateTexturing(true);
        }
        else if (test2)
        {
            test2 != test2;
            myEngine.activateTexturing(true);
            GLBI_Texture cellTexture{myTextureTest};
            cellTexture.attachTexture();
            cell.square->draw();
            cellTexture.detachTexture();
        }
        else
        {
            myEngine.activateTexturing(true);
            GLBI_Texture cellTexture{setTextureCell(cell, allTextures)}; // change based on cell.value
            cellTexture.attachTexture();
            cell.square->draw();
            cellTexture.detachTexture();
        }
    };
}

void renderScene()
{
    drawTexturedBaseMap(map, allTextures);
    // drawBaseMap(map);
    // drawAllCharacters(player);

    // static double lastTime = glfwGetTime();
    // double currentTime = glfwGetTime();
    // float deltaTime = static_cast<float>(currentTime - lastTime);
    // lastTime = currentTime;

    // updatePlayerPosition(map, deltaTime, player);
    // updatePlayerMesh(player);
    // updateEnemies(enemies, directedMap);
    // updateEnemiesMesh(enemies);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        activeKeys.insert(key);
    }
    else if (action == GLFW_RELEASE)
    {
        activeKeys.erase(key);
    }

    if (activeKeys.count(GLFW_KEY_UP))
    {
        player.direction = Direction::UP;
    }
    else if (activeKeys.count(GLFW_KEY_DOWN))
    {
        player.direction = Direction::DOWN;
    }
    else if (activeKeys.count(GLFW_KEY_LEFT))
    {
        player.direction = Direction::LEFT;
    }
    else if (activeKeys.count(GLFW_KEY_RIGHT))
    {
        player.direction = Direction::RIGHT;
    }
    else
    {
        player.direction = Direction::NONE;
    }

    /*if(isPositionUpdated) {
        updatePlayerMesh(player, CELLSIZE);

        auto valuedMap = createValuedMap(map, player);
        directedMap = createDirectedMap(valuedMap);
    }*/

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