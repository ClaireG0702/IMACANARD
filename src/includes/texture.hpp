#pragma once

#include "glbasimac/glbi_texture.hpp"
#include <vector>

#include "map.hpp"

using namespace glbasimac;

struct SpriteSheet
{
    char const *filename;
    unsigned int width;
    unsigned int height;
    unsigned int nbElOnX;
    unsigned int nbElOnY;
};

struct Sprite
{
    const SpriteSheet &spriteSheet;
    glm::vec2 positions;
    float spriteWidth;
    float spriteHeight;
};

GLBI_Texture createOneTexture(const char *filename);
std::vector<GLBI_Texture> initTextures();
const GLBI_Texture &setTextureCell(int indexTexture, std::vector<GLBI_Texture> const &allTextures);
void updateUVs(StandardMesh *mesh, Sprite const &sprite);
