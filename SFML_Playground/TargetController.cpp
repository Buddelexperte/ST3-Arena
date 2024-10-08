#include "TargetController.h"
#include <iostream>

void TargetController::newRandomConfig()
{
	TARGET_CONFIG.pos.x += 50;
}

void TargetController::clear()
{
	targets.clear();
	resetTimer();
}

void TargetController::resetTimer()
{
	spawnTimer->setCurrentTime(spawnTimer->getMaxTime());
	newRandomConfig();
}

void TargetController::update(const float& deltaTime)
{
	spawnTimer->update(deltaTime);
	if (isFinished())
	{
		targets.push_back(std::make_unique<Button>(TARGET_CONFIG));
		resetTimer();
	}
}

bool TargetController::isFinished() const
{
	return (spawnTimer->isFinished());
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
			it = targets.erase(it);
			delayScale += 0.02;
			spawnTimer->setMaxTime(spawnTimer->getMaxTime() / delayScale);
			resetTimer();
			return true;
		}
	}
	return false;
}