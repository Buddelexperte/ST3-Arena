#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Timer.h"
#include <random>
#pragma once

class TargetController : public sf::Drawable
{
private:
	ButtonConstruct TARGET_CONFIG = {
		sf::Vector2f{500.0f, 500.0f}, sf::Vector2f{100.0f, 100.0f}, sf::Color::White, 0, "", sf::Color::Transparent
	};
	
	std::vector<std::unique_ptr<Button>> targets;
	
	float windowWidth = 0.0f;
	float windowHeight = 0.0f;

	static std::random_device rd;

	void newRandomConfig();
	void clear();
	void spawnTarget();
public:

	TargetController() {};

	void initSpawner(sf::RenderTarget&);
	void update(sf::RenderTarget&);
	bool clickedAny(const sf::Vector2f&);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

