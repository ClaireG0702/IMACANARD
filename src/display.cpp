#include "includes/display.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include "tools/stb_image.h"
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

SpriteSheet water{"assets/images/Water.png", 192, 256, 12, 16};
SpriteSheet yellowDucky{"assets/images/yellow_ducky.png", 216, 144, 6, 4};
SpriteSheet brownDucky{"assets/images/brown_ducky.png", 216, 144, 6, 4};
// SpriteSheet otter{"assets/images/otter.png", 6, 3};

Sprite plain{water, {3, 9}, 16, 16};
Sprite empty{water, {3, 2}, 16, 16};

Sprite playerSprite{yellowDucky, {1, 0}, 36, 36};

// GLBI_Texture myTextureTest{};

void initScene()
{
    map = generateMap(width, width);
    map = generateCellularMap(map, 4);

    cellMesh = createSharedCellMesh(water, plain, cellSize, cellSize, 0.0f);
    addTextureBuffer(cellMesh);
    characterMesh = createSharedCellMesh(yellowDucky, playerSprite, characterSize, characterSize, 1.0f);
    addTextureBuffer(characterMesh);

    allTextures = initTextures();

    initPlayer(player, map);
    valuedMap = createValuedMap(map, player);
    directedMap = createDirectedMap(valuedMap);

    enemies = std::vector<Enemy>(numberOfEnemies);
    initEnemies(enemies, player, map);

    // TODO : define positions of obstacles
}

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
    case 3:
        myEngine.setFlatColor(0.7f, 0.6f, 1);
        break;
    case 4:
        myEngine.setFlatColor(0, 0.8f, 0.7f); 
        break;
    default:
        myEngine.setFlatColor(0, 0, 0); // Default
        break;
    }
}

/*
    Toute la texture                    Accéder à Un bout
          v                                y
    (u0,v1) ^ 1          (u1,v1)           ^
            |****TTTTTTT               ****|TTTTTTT
            |****TTTTTTT               ****|TTTTTTT
            |***********               ****|*******
            |***********               ****|*******
    (u0,v0) |*********** (u1,v0)       ****|*******
          -------------> u             ---------------> x = u_norm
          0              1

    Normalisation :
    1/nbRow = hauteur de * avec les axes u et v
    1/nbCol = largeur de * avec les axes u et v

    u0 = T.u * spriteWidth, u1 = u0 + spriteWidth
    v0 = T.v * spriteHeight, v1 = v0 + spriteHeight
*/

StandardMesh *createSharedCellMesh(SpriteSheet const &spritesheet, Sprite const &sprite, float cellWidth, float cellHeight, float z)
{
    StandardMesh *mesh = new StandardMesh(4, GL_TRIANGLE_FAN);
    std::vector<float> squareCoords{
        0.0f, 0.0f, z,
        cellWidth, 0.0f, z,
        cellWidth, cellHeight, z,
        0.0f, cellHeight, z};

    float u0{sprite.positions.x * sprite.spriteWidth / spritesheet.width};
    float u1{u0 + sprite.spriteWidth / spritesheet.width};
    float v0{sprite.positions.y * sprite.spriteHeight / spritesheet.height};
    float v1{v0 + sprite.spriteHeight / spritesheet.height};

    glm::vec2 bottomLeft{u0, v0};
    glm::vec2 bottomRight{u1, v0};
    glm::vec2 topRight{u1, v1};
    glm::vec2 topLeft{u0, v1};

    std::vector<float>
        uvs{
            bottomLeft.x, bottomLeft.y,   // Bottom Left (0,0)
            bottomRight.x, bottomRight.y, // Bottom Right (1,0)
            topRight.x, topRight.y,       // Top Right (1,1)
            topLeft.x, topLeft.y          // Top Left (0,1)
        };

    mesh->addOneBuffer(0, 3, squareCoords.data(), "coordinates", true);
    mesh->addOneBuffer(2, 2, uvs.data(), "uvs", true);
    mesh->createVAO();

    return mesh;
}

void updatePlayerMesh(Player &player)
{
    float x = -0.5f + player.position.x * cellSize;
    float y = -0.5f + player.position.y * cellSize;

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
        float x = -0.5f + enemy.position.x * cellSize;
        float y = -0.5f + enemy.position.y * cellSize;

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

        float x = -0.5f + cell.positions.x * cellSize;
        float y = -0.5f + cell.positions.y * cellSize;

        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({x, y, 0.0f});
        myEngine.updateMvMatrix();
        cellMesh->draw();
        myEngine.mvMatrixStack.popMatrix();
    }
};

void renderScene()
{
    //drawTexturedBaseMap(map, allTextures, cellSize, cellMesh);
    drawBaseMap(map);

    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;

    updatePlayerPosition(map, deltaTime, player);
    updatePlayerMesh(player);

    valuedMap = createValuedMap(map, player);
    directedMap = createDirectedMap(valuedMap);
    updateEnemies(enemies, directedMap, map, player, deltaTime);
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
        auto x{player.position.x};
        auto y{player.position.y};

        std::cout << "Try to dig" << std::endl;

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