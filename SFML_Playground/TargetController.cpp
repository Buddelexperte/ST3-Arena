#include "TargetController.h"
#include <iostream>

void TargetController::newRandomConfig()
{

	std::uniform_real_distribution<float> distrX(200.0f, windowWidth-200.0f);
	std::uniform_real_distribution<float> distrY(200.0f, windowHeight-200.0f);

	std::random_device rd;
	// Use the random device to seed a Mersenne Twister generator
	std::mt19937 gen(rd());

	TARGET_CONFIG.pos = sf::Vector2f(int(distrX(gen) / 100) * 100.0f, int(distrY(gen) / 100) * 100.0f);
}

void TargetController::clear()
{
	targets.clear();
}

void TargetController::update(sf::RenderTarget& window)
{
	windowWidth = window.getSize().x;
	windowHeight = window.getSize().y;
}

void TargetController::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& button : targets)
	{
		button->draw(target, states);
	}
}

bool TargetController::clickedAny(const sf::Vector2f& mousePos)
{
	for (auto it = targets.begin(); it != targets.end(); ++it)
	{
		if ((*it)->isClicked(mousePos))
		{
			targets.erase(it);
			spawnTarget();
			return true;
		}
	}
	return false;
}

void TargetController::spawnTarget()
{
	newRandomConfig();
	auto newButton = std::make_unique<Button>(TARGET_CONFIG);
	bool foundSpot = false;
	while (!foundSpot) {
		foundSpot = true; // Assume a valid spot is found until proven otherwise

		for (const auto& button : targets)
		{
			if (newButton->B_Box.getGlobalBounds().intersects(button->B_Box.getGlobalBounds()))
			{
				foundSpot = false; // Collision found, try again
				newRandomConfig();
				newButton->move(TARGET_CONFIG.pos);
				break; // Exit the for loop to check new position
			}
		}
	}

		
	targets.push_back(std::move(newButton));
}

void TargetController::initSpawner(sf::RenderTarget& window)
{
	update(window);
	targets.clear();
	for (int i = 0; i < 3; i++) spawnTarget();
}