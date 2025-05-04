#pragma once
#include <random>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Functions.h"

class RNG
{
private:
	static inline std::mt19937 rng = std::mt19937(std::random_device{}());

	RNG() 
		{}

	RNG(const RNG&) = delete;
	RNG& operator=(const RNG&) = delete;

public:
	// Acess Point here
	static RNG& getInstance() {
		static RNG instance;
		return instance;
	}

	// Float in range
	static float floatInRange(const float& min, const float& max)
	{
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng);
	}

	// Float in range
	static int intInRange(const int min, const int max)
	{
		float fMin = static_cast<float>(min);
		float fMax = static_cast<float>(max);

		int intRes = static_cast<int>(floatInRange(fMin, fMax));
		return intRes;
	}

	// Percentage in 0.0f to 1.0f notation
	static bool chance(const float& percentage)
	{
		return (floatInRange(0.0f, 1.0f) <= percentage);
	}

	// Position in box shape from down-left to up-right
	static sf::Vector2f posInRange(const sf::Vector2f& start, const sf::Vector2f& end)
	{
		float x = floatInRange(start.x, end.x);
		float y = floatInRange(start.y, end.y);

		return sf::Vector2f(x, y);
	}
	static sf::Vector2f posInDistanceFrom(const sf::Vector2f& center, const float& dist)
	{
		float angle = floatInRange(0.0f, 2.0f * static_cast<float>(pi));

		float x = center.x + dist * std::cos(angle);
		float y = center.y + dist * std::sin(angle);

		return sf::Vector2f(x, y);
	}
};