#pragma once
#include "Background.h" // Own header file
#include "RandomNumbers.h"

BackgroundElement::BackgroundElement(InputWidget* parent)
	: WidgetElement(parent), background(sf::Quads, 4)
{
	if (!tex_atlas.loadFromFile("Content/Textures/Tiles/background/background_atlas.png"))
	{
		std::cerr << "Failed to load background atlas!" << std::endl;
	}
	std::cout << "Atlas size: " << tex_atlas.getSize().x << " x " << tex_atlas.getSize().y << "\n";
	tex_atlas.setRepeated(true); // Very important for background tiling

	// Initialize the RNG just once
	seedGenerator.seed(std::random_device{}());

	// No default shape drawing
	shapes = { };
	updateVertexArray();
}

void BackgroundElement::construct()
{
	// Nothing dynamic here yet
}

void BackgroundElement::tick(const float& deltaTime)
{
	updateVertexArray();
}

void BackgroundElement::updateVertexArray()
{
	static constexpr float TILE_SIZE = TILE_WIDTH * TILING_SCALE;

	backgroundPos = viewCenter * parallaxStrength;

	// Use the class member variable for tile size
	const int tilesX = static_cast<int>(std::ceil(viewSize.x / TILE_SIZE)) + 2;
	const int tilesY = static_cast<int>(std::ceil(viewSize.y / TILE_SIZE)) + 2;

	// Compute top-left corner in world space
	float startX = viewCenter.x - viewSize.x / 2.0f;
	float startY = viewCenter.y - viewSize.y / 2.0f;
	int tileStartX = static_cast<int>(std::floor(startX / static_cast<float>(TILE_SIZE)));
	int tileStartY = static_cast<int>(std::floor(startY / static_cast<float>(TILE_SIZE)));

	background.setPrimitiveType(sf::Quads);
	background.resize(static_cast<size_t>(tilesX) * tilesY * 4);

	size_t vertexIndex = 0;
	for (int y = 0; y < tilesY; y++)
	{
		for (int x = 0; x < tilesX; x++)
		{
			int worldTileX = tileStartX + x;
			int worldTileY = tileStartY + y;
			float worldX = worldTileX * TILE_SIZE;
			float worldY = worldTileY * TILE_SIZE;

			// Get a consistent random tile based on world position
			int tileIndex = pseudoRandomTileIndex(worldTileX, worldTileY, 0, ATLAS_COLUMNS * ATLAS_ROWS - 1);
			int atlasX = tileIndex % ATLAS_COLUMNS;
			int atlasY = tileIndex / ATLAS_COLUMNS;

			// Calculate texture coordinates based on atlas position
			// Atlas tile size may differ from display tile size
			int atlasTileSize = tex_atlas.getSize().x / ATLAS_COLUMNS;  // Assuming square tiles in atlas
			sf::Vector2f texOffset(atlasX * atlasTileSize, atlasY * atlasTileSize);

			background[vertexIndex + 0].position = { worldX, worldY };
			background[vertexIndex + 1].position = { worldX + TILE_SIZE, worldY };
			background[vertexIndex + 2].position = { worldX + TILE_SIZE, worldY + TILE_SIZE };
			background[vertexIndex + 3].position = { worldX, worldY + TILE_SIZE };

			background[vertexIndex + 0].texCoords = texOffset;
			background[vertexIndex + 1].texCoords = texOffset + sf::Vector2f(atlasTileSize, 0);
			background[vertexIndex + 2].texCoords = texOffset + sf::Vector2f(atlasTileSize, atlasTileSize);
			background[vertexIndex + 3].texCoords = texOffset + sf::Vector2f(0, atlasTileSize);

			vertexIndex += 4;
		}
	}
}

void BackgroundElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, &tex_atlas);
}

int BackgroundElement::pseudoRandomTileIndex(int x, int y, int min, int max)
{
	// Create a deterministic hash from the world coordinates
	uint32_t hash = static_cast<uint32_t>((x * 73856093) ^ (y * 19349663));

	// Use the hash to get a seed for this specific tile
	std::mt19937 tileRng(hash);

	// Create a distribution based on the given min and max range
	std::uniform_int_distribution<int> dist(min, max);

	// Return a random tile index within the specified range
	return dist(tileRng);
}