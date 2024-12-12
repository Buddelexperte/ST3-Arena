#pragma once
#include "TargetController.h" // Own header file

void TargetController::newRandomConfig()
{
	windowUpdate();

	const float margin = 200.0f; // margin, so the targets dont overlap with healthbar or window edges
	
	sf::Vector2f upL = viewCorner(view, 1);
	sf::Vector2f downR = viewCorner(view, 4);

	sf::Vector2f playerPos = gameInstance.getPlayer()->getPos();
	float randomRadius = rng.floatInRange(500.0f, 1000.0f);

	sf::Vector2f generatedPos = rng.posInRadius(playerPos, randomRadius);
	sf::Vector2f velocity = gameInstance.getPlayer()->getVelocity();
	generatedPos += velocity * 10000.0f;

	sf::Vector2f gridPos = { std::trunc(generatedPos.x / 100), std::trunc(generatedPos.y / 100) };
	gridPos = 100.0f * gridPos;

	TARGET_CONFIG.pos = gridPos;
}

void TargetController::update(const float& deltaTime)
{
	static float timer = spawnInterval;
	windowUpdate();
	
	if (gameInstance.getIsPaused()) return;

	timer -= deltaTime;
	if (timer <= 0.0f)
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

bool TargetController::isHovering(const sf::Vector2f& mousePos)
{
	// Go through all targets with iterator pointing to each
	for (auto& target : targets)
	{
		if (target->isMouseOver()) return true;
	}
	return false; // No button was clicked
}

bool TargetController::clickedAny(const sf::Vector2f& mousePos)
{
	// Go through all targets with iterator pointing to each
	for (auto it = targets.begin(); it != targets.end(); ++it)
	{
		if ((*it)->isMouseOver())
		{
			targets.erase(it); // If one is clicked, remove it from the target list
			return true;
		}
	}
	return false; // No button was clicked
}

void TargetController::spawnTarget()
{
	newRandomConfig();
	std::unique_ptr<Button> newButton = std::make_unique<Button>(TARGET_CONFIG);
	bool foundSpot = true;
	for (int i = 0; i < 16 && !foundSpot; i++) {
		// Check if any already exitsing target is overlapping with that position
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
	// Move uniquePointer to targets vector using std::move (required for unique_ptr)
	targets.push_back(std::move(newButton));
}

void TargetController::initSpawner()
{
	targets.clear(); // Dereference old targets
	windowUpdate(); // Set window dimensions before spawning, so spawn positions are correct
	for (int i = 0; i < 3; i++) spawnTarget(); // Spawn 3 start targets
}