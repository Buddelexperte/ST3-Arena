#pragma once
#include <SFML/Graphics.hpp>

// SFML abbreviations
const sf::Keyboard::Key KEY_W = sf::Keyboard::W;
const sf::Keyboard::Key KEY_A = sf::Keyboard::A;
const sf::Keyboard::Key KEY_S = sf::Keyboard::S;
const sf::Keyboard::Key KEY_D = sf::Keyboard::D;
const sf::Keyboard::Key KEY_LSHIFT = sf::Keyboard::LShift;
// Mathematical Pi
const double pi = 4 * std::atan(1.0); // Calculation of Pi

// Global functions for calculation and conversion

inline float getLookAtRot(const sf::Vector2f& start, const sf::Vector2f& target)
{
	sf::Vector2f dir = target - start;
	float angle = std::atan2(dir.y, dir.x) * 180.0f / static_cast<float>(pi);
	return angle;
}

inline float smoothRot(const float& currentRot, const float& targetRot, float factor)
{
	float diff = targetRot - currentRot;

	// Wrap difference to the range [-180, 180] for degrees
	if (diff > 180.0f) diff -= 360.0f;
	if (diff < -180.0f) diff += 360.0f;

	return currentRot + factor * diff;
}

inline sf::Vector2f smoothUV(const sf::Vector2f& currentCenter, const sf::Vector2f& targetCenter, float factor)
{
	return currentCenter + factor * (targetCenter - currentCenter);
}