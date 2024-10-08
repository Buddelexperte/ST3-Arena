#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Timer.h"
#pragma once

class TargetController : public sf::Drawable
{
private:
	ButtonConstruct TARGET_CONFIG = {
		sf::Vector2f{500.0f, 500.0f}, sf::Vector2f{100.0f, 100.0f}, sf::Color::White, 0, "", sf::Color::Transparent
	};
	
	Timer* spawnTimer;
	std::vector<std::unique_ptr<Button>> targets;
	
	float delayScale = 1.0f;

	void newRandomConfig();
public:
	int hitTargets = 0;


	TargetController()
	{
		spawnTimer = new Timer(3.0f, 10, 10, sf::Vector2f{ 0, 0 }, sf::Color::Transparent);
	}

	void clear();
	void resetTimer();
	void update(const float&);
	bool clickedAny(const sf::Vector2f&);
	bool isFinished() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

