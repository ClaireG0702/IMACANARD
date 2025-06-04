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

std::vector<Cell> map; // the map to be displayed
StandardMesh *cellMesh = nullptr;
StandardMesh *characterMesh = nullptr;

std::set<int> activeKeys;

Player player{};
std::vector<Enemy> enemies;
std::vector<Cell> valuedMap;
std::vector<CellDirection> directedMap;

std::vector<GLBI_Texture> allTextures{};

GLBI_Texture myTextureTest{};

void initScene()
{
    map = generateMap(WIDTH, WIDTH);
    map = generateCellularMap(map, 4);

    cellMesh = createSharedCellMesh(CELLSIZE, CELLSIZE);
    characterMesh = createSharedCellMesh(CHARACTERSSIZE, CHARACTERSSIZE, 1.0f);
    allTextures = initTextures();

    initPlayer(player, map);
    valuedMap = createValuedMap(map, player);
    directedMap = createDirectedMap(valuedMap);

    enemies = std::vector<Enemy>(2);
    initEnemies(enemies, map);

    // TODO : define positions of obstacles
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
    cellPointer->addOneBuffer(2, 2, uvs.data(), "uvs", true);
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

    allTextures.reserve(filenames.size()); // we already know the size of allTextures
    for (char const *filename : filenames)
    {
        allTextures.push_back(createOneTexture(filename)); // createOneTexture creates a new Texture that is pushed at the end of the vector
    }

    return allTextures;
}

const GLBI_Texture &setTextureCell(Cell const &cell, std::vector<GLBI_Texture> const &allTextures)
{
    if (cell.value < 0 || cell.value >= allTextures.size())
    {
        std::cerr << "Error: Cell value out of range for textures." << std::endl;
        return allTextures.back(); // back = error texture --> to know visually something not ok
    }

    return allTextures[cell.value]; // TODO : find out how to use one part of sprites
};

void setTypeCell(Cell const &cell)
{
    switch (cell.value)
    {
    case 0:
        myEngine.setFlatColor(0, 0, 1); // Blue
        break;
    case 1:
        myEngine.setFlatColor(0, 1, 0); // Green
        break;
    default:
        myEngine.setFlatColor(0, 0, 0); // Default
        break;
    }
}

StandardMesh *createSharedCellMesh(float cellWidth, float cellHeight, float z)
{
    StandardMesh *mesh = new StandardMesh(4, GL_TRIANGLE_FAN);
    std::vector<float> squareCoords{
        0.0f, 0.0f, z,
        cellWidth, 0.0f, z,
        cellWidth, cellHeight, z,
        0.0f, cellHeight, z};

    std::vector<float> uvs{
        0.0f, 0.0f, // Bottom Left
        1.0f, 0.0f, // Bottom Right
        1.0f, 1.0f, // Top Right
        0.0f, 1.0f  // Top Left
    };

    mesh->addOneBuffer(0, 3, squareCoords.data(), "coordinates", true);
    mesh->addOneBuffer(2, 2, uvs.data(), "uvs", true);
    mesh->createVAO();

    return mesh;
}

void updatePlayerMesh(Player &player)
{
    float x = -0.5f + player.position.x * CELLSIZE;
    float y = -0.5f + player.position.y * CELLSIZE;

    myEngine.setFlatColor(1.0f, 0.0f, 0.0f);
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation({x, y, 0.0f});
    myEngine.updateMvMatrix();
    characterMesh->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void updateEnemiesMesh(std::vector<Enemy> &enemies)
{
    for (Enemy &enemy : enemies)
    {
        float x = -0.5f + enemy.position.x * CELLSIZE;
        float y = -0.5f + enemy.position.y * CELLSIZE;

        myEngine.setFlatColor(1.0f, 1.0f, 0.0f);
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({x, y, 0.0f});
        myEngine.updateMvMatrix();
        characterMesh->draw();
        myEngine.mvMatrixStack.popMatrix();
    }
}

void drawBaseMap(std::vector<Cell> const &map)
{
    for (Cell const &cell : map)
    {
        setTypeCell(cell);

        float x = -0.5f + cell.positions.x * CELLSIZE;
        float y = -0.5f + cell.positions.y * CELLSIZE;

        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({x, y, 0.0f});
        myEngine.updateMvMatrix();
        cellMesh->draw();
        myEngine.mvMatrixStack.popMatrix();
    }
};

void drawTexturedBaseMap(std::vector<Cell> const &map, std::vector<GLBI_Texture> const &allTextures)
{
    bool test1{true};
    bool test2{true};
    for (Cell const &cell : map)
    {
        setTypeCell(cell);

        float x = -0.5f + cell.positions.x * CELLSIZE;
        float y = -0.5f + cell.positions.y * CELLSIZE;

        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({x, y, 0.0f});
        myEngine.updateMvMatrix();

        if (test1)
        {
            test1 = !test1;
            myEngine.activateTexturing(false);
            myEngine.setFlatColor(1.0, 0.0, 1.0);
            cellMesh->draw();

            myEngine.activateTexturing(true);
        }
        else if (test2)
        {
            test2 != test2;
            myEngine.activateTexturing(true);
            const GLBI_Texture &cellTexture{setTextureCell(cell, allTextures)};
            cellTexture.attachTexture();
            cellMesh->draw();
            cellTexture.detachTexture();
        }
        else
        {
            myEngine.activateTexturing(true);
            // GLBI_Texture cellTexture{setTextureCell(cell, allTextures)}; // change based on cell.value
            GLBI_Texture &cellTexture{const_cast<GLBI_Texture &>(setTextureCell(cell, allTextures))}; // use references
            cellTexture.attachTexture();
            cellMesh->draw();
            cellTexture.detachTexture();
        }

        myEngine.mvMatrixStack.popMatrix();
    };
}

void renderScene()
{
    //drawTexturedBaseMap(map, allTextures);
    drawBaseMap(map);

    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;

    updatePlayerPosition(map, deltaTime, player);
    updatePlayerMesh(player);

    valuedMap = createValuedMap(map, player);
    directedMap = createDirectedMap(valuedMap);
    updateEnemies(enemies, directedMap, map, deltaTime);
    updateEnemiesMesh(enemies);
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

    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
    { // Quand on appuie sur 'A', on mine le bloc devant nous
        auto x{player.gridPos.x};
        auto y{player.gridPos.y};

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