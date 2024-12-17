#pragma once
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Functions.h"

class RNG
{
private:
	std::mt19937 rng;

	RNG(): rng(std::random_device{}()) {}

	RNG(const RNG&) = delete;
	RNG& operator=(const RNG&) = delete;

public:
	// Acess Point here
	static RNG& getInstance() {
		static RNG instance;
		return instance;
	}

	// Float in range
	float floatInRange(const float& min, const float& max)
	{
		std::uniform_real_distribution<float> dist(min, max);
		return dist(this->rng);
	}

	bool roulette(const float& percentage)
	{
		return (floatInRange(0.0f, 100.0f) <= percentage);
	}

	// Position in box shape from down-left to up-right
	sf::Vector2f posInRange(const sf::Vector2f& start, const sf::Vector2f& end)
	{
		float x = floatInRange(start.x, end.x);
		float y = floatInRange(start.y, end.y);

		return sf::Vector2f(x, y);
	}
	sf::Vector2f posInDistance(const sf::Vector2f& center, const float& dist)
	{
		float angle = floatInRange(0.0f, 2.0f * static_cast<float>(pi));

		float x = center.x + dist * std::cos(angle);
		float y = center.y + dist * std::sin(angle);

		return sf::Vector2f(x, y);
	}
};