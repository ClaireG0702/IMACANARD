#include "includes/texture.hpp"
#include "includes/display.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"

GLBI_Texture createOneTexture(const char *filename)
{
    int x{};
    int y{};
    int n{};

    stbi_set_flip_vertically_on_load(true);
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

    std::array<char const *, 5> filenames{"assets/images/Water.png",
                                          "assets/images/yellow_ducky.png",
                                          "assets/images/brown_ducky.png",
                                          "assets/images/otter.png",
                                          "assets/images/error.png"};

    allTextures.reserve(filenames.size()); // we already know the size of allTextures
    for (char const *filename : filenames)
    {
        allTextures.push_back(createOneTexture(filename)); // createOneTexture creates a new Texture that is pushed at the end of the vector
    }

    return allTextures;
}

const GLBI_Texture &setTextureCell(int indexTexture, std::vector<GLBI_Texture> const &allTextures)
{
    if (indexTexture < 0 || indexTexture >= allTextures.size())
    {
        std::cerr << "Error: Cell value out of range for textures." << std::endl;
        return allTextures.back(); // back = error texture --> to know visually something not ok
    }

    return allTextures[indexTexture]; // TODO : find out how to use one part of sprites
};

void drawTexturedBaseMap(std::vector<Cell> const &map, std::vector<GLBI_Texture> const &allTextures, float cellSize, StandardMesh *cellMesh)
{
    for (Cell const &cell : map)
    {
        setTypeCell(cell);

        float x = -0.5 + cell.positions.x * cellSize;
        float y = -0.5 + cell.positions.y * cellSize;

        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTranslation({x, y, 0.0f});
        myEngine.updateMvMatrix();

        myEngine.activateTexturing(true);
        GLBI_Texture &cellTexture{const_cast<GLBI_Texture &>(setTextureCell(cell.value, allTextures))}; // use references
        cellTexture.attachTexture();
        cellMesh->draw();
        cellTexture.detachTexture();

        myEngine.mvMatrixStack.popMatrix();
    };
}

void updateUVs(StandardMesh *mesh, Sprite const &sprite)
{
    float u0{sprite.spriteWidth * sprite.positions.x};
    float u1{u0 + sprite.spriteWidth};
    float v0{sprite.spriteHeight * sprite.positions.y};
    float v1{v0 + sprite.spriteHeight};

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

    mesh->updateBuffer(2, uvs.data(), 8);
}