#pragma once

#include "WidgetElements.h"
#include <random>

class BackgroundElement : public WidgetElement
{
private:
    // Constants
    static constexpr float TILING_SCALE = 5.0f;
    static constexpr int TILE_WIDTH = 16;       // Pixels per Tile
    static constexpr int ATLAS_COLUMNS = 4;     // Number of tiles horizontally in the atlas
    static constexpr int ATLAS_ROWS = 1;        // Number of tiles vertically in the atlas

    // Member variables
    sf::Texture tex_atlas;
    sf::VertexArray background;
    sf::Vector2f backgroundPos;
    float parallaxStrength = 0.5f;

    // Random number generation
    std::mt19937 seedGenerator;

    // Methods
    void updateVertexArray();
    int pseudoRandomTileIndex(int x, int y, int min, int max);

public:
    BackgroundElement(InputWidget* parent);
    void construct() override;
    void tick(const float& deltaTime) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray& getVertexArray()
    {
        return background;
    }

};