#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Timer.h"
#include <random>
#pragma once

// Class for handling and spawning targets the user can click on
class TargetController : public sf::Drawable
{
private:
	ButtonConstruct TARGET_CONFIG = {
		sf::Vector2f{500.0f, 500.0f}, sf::Vector2f{100.0f, 100.0f}, sf::Color::White, 0, "", sf::Color::Transparent
	}; // A white button constructor with no text, 100x100
	
	std::vector<std::unique_ptr<Button>> targets;
	
	float windowWidth = 0.0f;
	float windowHeight = 0.0f;


	void newRandomConfig(); // New TARGET_CONFIG using random distr position
	void spawnTarget(); // Spawn a new target using newRandomConfig() inside
public:
	void initSpawner(sf::RenderTarget&); // Spawn first three targets, reset all old values
	void update(sf::RenderTarget&); // Update window dimensions
	bool clickedAny(const sf::Vector2f&); // Check if any button got clicked and erase it

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Draw all target buttons
};

