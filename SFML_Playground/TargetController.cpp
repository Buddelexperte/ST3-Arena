#pragma once
#include "TargetController.h" // Own header file
#include "BaseClasses.h"

TargetController::TargetController() : gen(rd())
{

}

void TargetController::newRandomConfig()
{
	windowUpdate();

	const float margin = 200.0f; // margin, so the targets dont overlap with healthbar or window edges
	
	sf::Vector2f upL = viewCorner(view, 1);
	sf::Vector2f downR = viewCorner(view, 4);

	std::uniform_real_distribution<float> distrX(upL.x + margin, downR.x - margin); // Distr for X coordinates based on actual windowWidth
	std::uniform_real_distribution<float> distrY(upL.y + margin, downR.y - margin); // Distr for Y coordinates based on actual windowHeight
	// Set TARGET_CONFIG to generation of distrX and Y but mapped to a 100x100 grid
	sf::Vector2f generatedPos = { distrX(gen), distrY(gen) };

	generatedPos += gameInstance.getPlayer()->getDirection() * 1000.0f;

	sf::Vector2f gridPos = { std::trunc(generatedPos.x / 100), std::trunc(generatedPos.y / 100) };
	gridPos = 100.0f * gridPos;

	TARGET_CONFIG.pos = gridPos;
	//TARGET_CONFIG.pos = sf::Vector2f(int(distrX(gen) / 100) * 100.0f, int(distrY(gen) / 100) * 100.0f);
}

void TargetController::windowUpdate()
{
	view = gameInstance.getView();
	viewCenter = view->getCenter();
	viewHalfSize = view->getSize() / 2.0f;
}

void TargetController::update(const float& deltaTime)
{
	static float timer = 1.0f;
	windowUpdate();
	
	if (gameInstance.getIsPaused()) return;
	timer -= deltaTime;
	if (timer <= 0.0f)
	{
		spawnTarget();
		timer = 1.0f;
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
	for (const std::unique_ptr<Button>& target : targets)
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
			spawnTarget(); // Spawn a new target, using random distribution
			return true;
		}
	}
	return false; // No button was clicked
}

void TargetController::spawnTarget()
{
	newRandomConfig();
	auto newButton = std::make_unique<Button>(TARGET_CONFIG);
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