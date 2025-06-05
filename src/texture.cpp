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

    return allTextures[indexTexture];
};

void updateUVs(StandardMesh *mesh, Sprite const &sprite)
{
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

    mesh->updateBuffer(2, uvs.data(), 8);
}