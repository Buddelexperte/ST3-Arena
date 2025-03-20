#pragma once
#include "TargetController.h" // Own header file
#include "GameInstance.h"

void TargetController::newRandomConfig()
{
	updateValues();

	// Getting values for generating a position
	sf::Vector2f playerPos = gameInstance().getPlayer()->getPosition();
	float distance = rng.floatInRange(600.0f, 1000.0f);

	// Generating the random position
	sf::Vector2f generatedPos = rng.posInDistanceFrom(playerPos, distance);
	sf::Vector2f velocity = gameInstance().getPlayer()->getVelocity();
	generatedPos += velocity * 10.0f; // Take velocity into calculation for "movement prediction"
	
	// Snapping the position on a grid (100 x 100)
	sf::Vector2f gridPos = posToGrid(generatedPos, 100.0f);
	
	// Setting the new Button Target Position in the ButtonConfig variable for later use inside spawnTarget()
	TARGET_CONFIG.pos = gridPos;
}

void TargetController::tick(const float& deltaTime)
{
	WidgetElement::tick(deltaTime);
	static float timer = spawnInterval;
	
	if (gameInstance().getIsPaused()) return;

	if (numTargetsAlive > 10) return;

	timer -= deltaTime;
	if (timer < 0.0f)
	{
		spawnTarget();
		timer = 0.1f;
		return;
	}
}

void TargetController::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& button : targets) // Every target
	{
		button->draw(target, states); // Call the draw function for every button created
	}
}

bool TargetController::isHovering()
{
	// Go through all targets with iterator pointing to each
	for (const auto& target : targets)
	{
		if (target->isMouseOver()) return true;
	}
	return false; // No button was clicked
}

bool TargetController::clickedAny()
{
	// Go through all targets with iterator pointing to each
	for (std::vector<std::unique_ptr<Button>>::iterator it = targets.begin(); it != targets.end(); /*No Increment*/)
	{
		if ((*it)->isMouseOver(true))
		{
			targets.erase(it); // If one is clicked, remove it from the target list
			numTargetsAlive--;
			return true;
		}
		else it++;
	}
	return false; // No button was clicked
}

void TargetController::spawnTarget()
{
	newRandomConfig();
	std::unique_ptr<Button> newButton = std::make_unique<Button>(TARGET_CONFIG);
	bool foundSpot = false;
	for (int i = 0; i < 4 && !foundSpot; i++) {
		// Check if any already exitsing target is overlapping with that position
		foundSpot = true;
		for (const auto& button : targets)
		{
			if (newButton->B_Box.getGlobalBounds().intersects(button->B_Box.getGlobalBounds()))
			{
				foundSpot = false; // Collision found, try again
				newRandomConfig(); // Make new position
				newButton->setPos(TARGET_CONFIG.pos); // Moves newButton to new position
				break; // Exit the for loop to check new position
			}
		}
	}
	if (!foundSpot) return;
	// Move uniquePointer to targets vector using std::move (required for unique_ptr)
	targets.push_back(std::move(newButton));
	numTargetsAlive++;
}

void TargetController::initSpawner()
{
	targets.clear(); // Dereference old targets
}