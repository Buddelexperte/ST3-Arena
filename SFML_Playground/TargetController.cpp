#include "TargetController.h"
#include <iostream>

void TargetController::newRandomConfig()
{
	std::random_device rd;
	// Use the random device to seed a Mersenne Twister generator
	std::mt19937 gen(rd());

	std::uniform_int_distribution<float> distrX(200.0f, windowWidth-200.0f);
	std::uniform_int_distribution<float> distrY(200.0f, windowHeight-200.0f);

	TARGET_CONFIG.pos = sf::Vector2f(distrX(gen), distrY(gen));
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
	targets.push_back(std::make_unique<Button>(TARGET_CONFIG));
}

void TargetController::initSpawner()
{
	targets.clear();
	for (int i = 0; i < 3; i++) spawnTarget();
}