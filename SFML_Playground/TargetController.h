#pragma once
#include "BaseClasses.h"
#include <random> // Random target position generation

// Class for handling and spawning targets the user can click on
class TargetController : public sf::Drawable
{
private:
	GI_Arena& gameInstance = GI_Arena::getInstance();
	ButtonConstruct TARGET_CONFIG = {
		sf::Vector2f{500.0f, 500.0f}, sf::Vector2f{100.0f, 100.0f}, sf::Color::White, 0, "", sf::Color::Transparent
	}; // A white button constructor with no text, 100x100
	
	std::vector<std::unique_ptr<Button>> targets;
	
	float windowWidth = 0.0f;
	float windowHeight = 0.0f;


	void newRandomConfig(); // New TARGET_CONFIG using random distr position
	void spawnTarget(); // Spawn a new target using newRandomConfig() inside
public:
	void initSpawner(); // Spawn first three targets, reset all old values
	void windowUpdate(); // Update window dimensions
	bool isHovering(const sf::Vector2f&);
	bool clickedAny(const sf::Vector2f&); // Check if any button got clicked and erase it

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Draw all target buttons
};