#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Timer.h"
#pragma once
class TargetController : public sf::Drawable
{
private:
	Timer* spawnTimer;
	std::vector<Button*> targets;
public:
	TargetController()
	{
		spawnTimer = new Timer(3.0f, 10, 10, sf::Vector2f{ 0, 0 }, sf::Color::Transparent);
	}

	void stop();
	void update(const float&);
	bool isFinished() const;
};

