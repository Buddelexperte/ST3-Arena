#pragma once
#include <SFML/Graphics.hpp>

const double pi = 4 * std::atan(1.0); // This calculates pi accurately

inline float getLookAtRot(const sf::Vector2f& start, const sf::Vector2f& target)
{
	sf::Vector2f dir = target - start;
	float angle = std::atan2(dir.y, dir.x) * 180.0f / static_cast<float>(pi);
	return angle;
}