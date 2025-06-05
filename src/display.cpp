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
StandardMesh *pngMesh = nullptr;

std::set<int> activeKeys;

Player player{};
std::vector<Enemy> enemies;
std::vector<Cell> valuedMap;
std::vector<CellDirection> directedMap;

std::vector<GLBI_Texture> allTextures{};

SpriteSheet water{"assets/images/Water.png", 192, 224, 12, 16};
SpriteSheet yellowDucky{"assets/images/yellow_ducky.png", 192, 128, 6, 4};
SpriteSheet brownDucky{"assets/images/brown_ducky.png", 192, 128, 6, 4};
SpriteSheet otterSheet{"assets/images/otter.png", 96, 64, 3, 2};

Sprite empty{water, {2, 11.55}, 1 / 12.f, 1 / 16.f};      // water --> 0
Sprite plain{water, {10.95, 9.25}, 1 / 12.f, 1 / 16.f};   // lilypad --> 1
Sprite obstacle{water, {0, 4.72}, 1 / 12.f, 1 / 16.f};    // rock -> 2
Sprite acceleration{water, {6, 3.5}, 1 / 12.f, 1 / 16.f}; // light --> 3
Sprite object{water, {8, 11.5}, 1 / 12.f, 1 / 16.f};      // shadow --> 4
Sprite trap{water, {1, 0}, 1 / 12.f, 1 / 16.f};           // whirlpool --> 5
Sprite holdup{water, {11, 11.5}, 1 / 12.f, 1 / 16.f};     // reed --> 6

Sprite playerSprite{yellowDucky, {1, 0}, 1.0 / 6, 1.0 / 4}; // yellow_ducky
Sprite ennemySprite{otterSheet, {1, 1}, 1.0 / 3, 1.0 / 2};  // otter

// GLBI_Texture myTextureTest{};

void initScene()
{
    cellSize = 1.0f / width;
    characterSize = 1.0f / (width * 2);
    numberOfObjects = width / 2;

    map = generateMap(width, width);
    map = generateCellularMap(map, 4);
    allTextures = initTextures();

    cellMesh = createSharedCellMesh(empty, cellSize, cellSize);
    characterMesh = createSharedCellMesh(playerSprite, characterSize, characterSize);
    pngMesh = createSharedCellMesh(plain, cellSize, cellSize); // modified line

    initPlayer(player, map);
    valuedMap = createValuedMap(map, player);
    directedMap = createDirectedMap(valuedMap);

    enemies = std::vector<Enemy>(numberOfEnemies);
    initEnemies(enemies, player, map);
}

void setTypeCell(Cell const &cell)
{
    switch (cell.value)
    {
    case 0:
        myEngine.setFlatColor(0, 0, 0.5f); // Blue
        break;
    case 1:
        myEngine.setFlatColor(0, 1, 0); // Green
        break;
    case 2:
        myEngine.setFlatColor(0, 0.2f, 0);
        break;
    case 3:
        myEngine.setFlatColor(0.6f, 0.7f, 0);
        break;
    case 4:
        myEngine.setFlatColor(0, 0, 0.2f);
        break;
    case 5:
        myEngine.setFlatColor(0, 0, 1);
        break;
    case 6:
        myEngine.setFlatColor(0.3f, 0, 0.2f);
        break;
    default:
        myEngine.setFlatColor(0, 0, 0); // Default
        break;
    }
}
/*
    All texture
              v
    (uTL,vTL) ^ 1          (uTR,vTR)
              |****TTTTTTT
              |****TTTTTTT
              |***********
              |***********
    (uBL,vBL) |*********** (uBR,BR)
              -------------> u
             0              1

    Sprite defined as TL : top left    | TR : top right
                    ___________________|_________________
                                       |
                     BL : bottom left  | BR : bottom right

    uXX is anything on u axis, vXX is anything on v axis
    Each corner is determined by BL and TR.

    spriteWidth = 1/number of elements on u axis
    spriteHeight = 1/number of elements on v axis

    uTL = uBL                                                 | uTR = uBL + spriteWidth
    vTL = vTR                                                 | vTR = vBL + spriteHeight
    __________________________________________________________|____________________________
    uBL = spriteWidth * column index (aka x) of the sprite    | uBR = uTR
    vBL = spriteHeight * row index (aka y) of the sprite      | vBR = vBL

*/

StandardMesh *createSharedCellMesh(Sprite const &sprite, float cellWidth, float cellHeight)
{
    StandardMesh *mesh = new StandardMesh(4, GL_TRIANGLE_FAN);
    std::vector<float> squareCoords{
        0.0f, 0.0f,
        cellWidth, 0.0f,
        cellWidth, cellHeight,
        0.0f, cellHeight};

    float uBL{sprite.spriteWidth * sprite.positions.x};
    float uTR{uBL + sprite.spriteWidth};
    float vBL{sprite.spriteHeight * sprite.positions.y};
    float vTR{vBL + sprite.spriteHeight};

    glm::vec2 bottomLeft{uBL, vBL};
    glm::vec2 bottomRight{uTR, vBL};
    glm::vec2 topRight{uTR, vTR};
    glm::vec2 topLeft{uBL, vTR};

    std::vector<float>
        uvs{
            bottomLeft.x, bottomLeft.y,   // Bottom Left (0,0)
            bottomRight.x, bottomRight.y, // Bottom Right (1,0)
            topRight.x, topRight.y,       // Top Right (1,1)
            topLeft.x, topLeft.y          // Top Left (0,1)
        };

    mesh->addOneBuffer(0, 2, squareCoords.data(), "coordinates", true);
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
    const GLBI_Texture &characterTexture{setTextureCell(1, allTextures)};
    characterTexture.attachTexture();
    characterMesh->draw();
    characterTexture.detachTexture();
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

        const GLBI_Texture &ennemyTexture{setTextureCell(3, allTextures)};
        updateUVs(characterMesh, ennemySprite);
        ennemyTexture.attachTexture();
        characterMesh->draw();
        ennemyTexture.detachTexture();
        updateUVs(characterMesh, playerSprite);

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

        // myEngine.activateTexturing(true);
        GLBI_Texture &cellTexture{const_cast<GLBI_Texture &>(setTextureCell(0, allTextures))}; // use references

        cellTexture.attachTexture();
        cellMesh->draw();
        cellTexture.detachTexture();

        // switch (cell.value)
        // {
        // case 1: // Non minable bloc
        // {
        //     GLBI_Texture &pngTexture{const_cast<GLBI_Texture &>(setTextureCell(0, allTextures))}; // use references
        //     updateUVs(pngMesh, rock);
        //     pngTexture.attachTexture();
        //     pngMesh->draw();
        //     pngTexture.detachTexture();
        //     updatesUVS(pngMesh, )
        //     break;
        // }

        // default:
        //     break;
        // }
        if (cell.value == 1)
        {
            GLBI_Texture &pngTexture{const_cast<GLBI_Texture &>(setTextureCell(0, allTextures))}; // use references
            // updateUVs(pngMesh, plain);
            cellTexture.attachTexture();
            pngMesh->draw();
            cellTexture.detachTexture();
            // updateUVs(cellMesh, empty);
        }

        myEngine.mvMatrixStack.popMatrix();
    }
};

void renderScene()
{
    // drawTexturedBaseMap(map, allTextures, cellSize, cellMesh);
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

void clearScene()
{
    if (cellMesh != nullptr)
    {
        delete cellMesh;
        cellMesh = nullptr;
    }

    if (characterMesh != nullptr)
    {
        delete characterMesh;
        characterMesh = nullptr;
    }

    map.clear();
    enemies.clear();
    valuedMap.clear();
    directedMap.clear();
    // allTextures.clear();

    player = Player{};
    enemies = std::vector<Enemy>();
}
