#pragma once
#include "Button.h"
#include <random> // Random target position generation

// Class for handling and spawning targets the user can click on
class TargetController : public sf::Drawable
{
private:
	GI_Arena& gameInstance = GI_Arena::getInstance();
	sf::View* view = nullptr;
	sf::Vector2f viewCenter = { 0.0f, 0.0f };
	sf::Vector2f viewHalfSize = { 0.0f, 0.0f };

	ButtonConstruct TARGET_CONFIG = {
		sf::Vector2f{500.0f, 500.0f}, sf::Vector2f{100.0f, 100.0f}, sf::Color::White, 0, "", sf::Color::Transparent
	}; // A white button constructor with no text, 100x100
	
	std::vector<std::unique_ptr<Button>> targets;
	
	std::random_device rd; // Get device random Number generator
	std::mt19937 gen; // Seed random number generator

	void newRandomConfig(); // New TARGET_CONFIG using random distr position
	void spawnTarget(); // Spawn a new target using newRandomConfig() inside
public:
	TargetController();
	void initSpawner(); // Spawn first three targets, reset all old values
	void windowUpdate(); // Update window dimensions
	void update(const float&);
	bool isHovering(const sf::Vector2f&);
	bool clickedAny(const sf::Vector2f&); // Check if any button got clicked and erase it

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Draw all target buttons
};