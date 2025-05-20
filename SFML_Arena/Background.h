#pragma once

#include "WidgetElements.h"
#include <random>

enum class EBackgroundTexture {
    DEBUG = -1,
    DEFAULT = 0,
    STONE,
    DIRT,
    WOOD
};

class BackgroundElement : public WidgetElement
{
public:

    BackgroundElement(InputWidget* parent);
    void construct() override;
    void tick(const float& deltaTime) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray& getVertexArray()
    {
        return background;
    }

    void setBackgroundTexture(const EBackgroundTexture&);
    EBackgroundTexture getBackgroundTexture() const;

private:
    EBackgroundTexture currBackground = EBackgroundTexture::STONE;

    // Atlas Constants
    static constexpr float TILING_SCALE = 5.0f;
    int TILE_WIDTH = 16;       // Pixels per Tile
    int ATLAS_COLUMNS = 4;     // Number of tiles horizontally in the atlas
    int ATLAS_ROWS = 1;        // Number of tiles vertically in the atlas

    // Member variables
    static constexpr unsigned int NUM_ATLAS_TO_LOAD = 4;
    std::vector<sf::Texture> loadedTextures;
    sf::Texture* usedAtlas = nullptr;
    sf::VertexArray background;
    sf::Vector2f backgroundPos;
    float parallaxStrength = 1.0f;

    // Random number generation
    std::mt19937 seedGenerator;

    // Methods
    void updateVertexArray();
    int pseudoRandomTileIndex(int x, int y, int min, int max);

};