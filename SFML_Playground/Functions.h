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


// Lock weak pointer into shared pointer
template <typename T>
std::shared_ptr<T> lockWeakPtr(const std::weak_ptr<T>& weakPtr) {
	if (auto lockedPtr = weakPtr.lock()) {
		return lockedPtr;
	}
	// std::cerr << "Weak pointer has expired and is unavailable!\n"; // Somehow doesn't recognize std:: namespace
	return nullptr;  // Or handle the expired object case appropriately
}


constexpr float SMALLEST_PRECISION = 1e-3f;

inline bool shouldZero(const float& value, const float& precision = SMALLEST_PRECISION)
{
	const float epsilon = precision;
	return (std::abs(value) < epsilon);
}

inline bool shouldZero(const sf::Vector2f& value, const float& precision = SMALLEST_PRECISION)
{
	return (shouldZero(value.x, precision) && shouldZero(value.y, precision));
}

// Clamp "too small" float values to 0.0f
inline void zeroPrecision(float& value, const float& precision = SMALLEST_PRECISION)
{
	if (shouldZero(value, precision)) value = 0.0f;
}

inline void zeroPrecision(sf::Vector2f& value, const float& precision = SMALLEST_PRECISION)
{
	zeroPrecision(value.x, precision);
	zeroPrecision(value.y, precision);
}


// LERP -------------------------------------------------------------------------------------------
inline float lerp(const float& currentRot, const float& targetRot, float factor)
{
	float diff = targetRot - currentRot;

	// Wrap difference to the range [-180, 180] for degrees
	if (diff > 180.0f) diff -= 360.0f;
	if (diff < -180.0f) diff += 360.0f;

	return currentRot + factor * diff;
}

inline sf::Vector2f lerp(const sf::Vector2f& currentCenter, const sf::Vector2f& targetCenter, float factor)
{
	return currentCenter + factor * (targetCenter - currentCenter);
}


// VECTOR2F MATH ----------------------------------------------------------------------------------
// Compatre Vector2f
inline bool operator==(const sf::Vector2f& vec1, const sf::Vector2f& vec2) {
	return (vec1.x == vec2.x && vec1.y == vec2.y);
}

// Vector2f * float
inline sf::Vector2f operator*(const sf::Vector2f& vec, float scalar) {
	return sf::Vector2f(vec.x * scalar, vec.y * scalar);
}

// Vector2f * Vector2f
inline sf::Vector2f operator*(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {
	return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y);
}

// Snapping a sf::Vector2f onto a grid of squares (sideLength is gridSize)
inline sf::Vector2f posToGrid(const sf::Vector2f& pos, const float& gridSize)
{
	sf::Vector2f snappedPos = { std::trunc(pos.x / gridSize), std::trunc(pos.y / gridSize) };
	snappedPos *= gridSize;
	return snappedPos;
}


// ROTATION ---------------------------------------------------------------------------------------
constexpr float degreesToRadians(float degrees) {
	return degrees * static_cast<float>(pi) / 180.0f;
}

inline float getLookAtRot(const sf::Vector2f& start, const sf::Vector2f& target)
{
	sf::Vector2f dir = target - start;
	float angle = std::atan2(dir.y, dir.x) * 180.0f / static_cast<float>(pi);
	return angle;
}