#pragma once
#include "Button.h"
#include "GameInstance.h"

// Class for handling and spawning targets the user can click on
class TargetController : public WidgetElement
{
private:
	RNG& rng = RNG::getInstance();

	const float spawnInterval = 0.1f;
	int numTargetsAlive = 0;
	std::vector<std::unique_ptr<Button>> targets;
	
	ButtonConstruct TARGET_CONFIG = {
		sf::Vector2f{500.0f, 500.0f}, sf::Vector2f{100.0f, 100.0f}, sf::Color::White, 0, "", sf::Color::Transparent
	}; // A white button constructor with no text, 100x100
	
	
	void newRandomConfig(); // New TARGET_CONFIG using random distr position
	void spawnTarget(); // Spawn a new target using newRandomConfig() inside
public:
	TargetController(InputWidget* parent) : WidgetElement(parent) {}
	void initSpawner(); // Spawn first three targets, reset all old values
	void tick(const float&) override;
	bool isHovering();
	bool clickedAny(); // Check if any button got clicked and erase it

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Draw all target buttons
};