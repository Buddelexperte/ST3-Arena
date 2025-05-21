#pragma once

#include <SFML/Graphics.hpp>

// SFML abbreviations
static const inline sf::Keyboard::Key KEY_W = sf::Keyboard::W;
static const inline sf::Keyboard::Key KEY_A = sf::Keyboard::A;
static const inline sf::Keyboard::Key KEY_S = sf::Keyboard::S;
static const inline sf::Keyboard::Key KEY_D = sf::Keyboard::D;
static const inline sf::Keyboard::Key KEY_LSHIFT = sf::Keyboard::LShift;
static const inline sf::Keyboard::Key KEY_ESC = sf::Keyboard::Escape;
static const inline sf::Keyboard::Key KEY_TAB = sf::Keyboard::Tab;

// Constants
static const inline double pi = 4.0f * std::atan(1.0); // Calculation of Pi
static constexpr inline float SMALLEST_PRECISION = 1e-3f; // Smallest precision for most floating point comparisons used

// WEAK POINTER -------------------------------------------------------------------------------------------
// Lock weak pointer into shared pointer
template <typename T>
std::shared_ptr<T> lockWeakPtr(const std::weak_ptr<T>& weakPtr)
{
	if (auto lockedPtr = weakPtr.lock()) {
		return lockedPtr;
	}
	// std::cerr << "Weak pointer has expired and is unavailable!\n"; // Somehow doesn't recognize std:: namespace
	return nullptr;  // Or handle the expired object case appropriately
}

// STRING -------------------------------------------------------------------------------------------
inline std::string toRoman(unsigned int number)
{
	static const std::pair<unsigned int, const char*> romanMap[] = {
		{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
		{100, "C"},  {90, "XC"},  {50, "L"},  {40, "XL"},
		{10, "X"},   {9, "IX"},   {5, "V"},   {4, "IV"}, {1, "I"}
	};

	std::string result;
	for (const auto& [value, numeral] : romanMap) {
		while (number >= value) {
			number -= value;
			result += numeral;
		}
	}
	return result;
}

// ROTATION ---------------------------------------------------------------------------------------
inline float getShortestAngle(float from, float to)
{
	float diff = fmod(to - from + 540.f, 360.f) - 180.f;
	return diff;
}


// PRECISION --------------------------------------------------------------------------------------
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

// CLAMP ------------------------------------------------------------------------------------------
inline void clamp(float& value, const float& min, const float& max)
{
	if (value < min) value = min;
	if (value > max) value = max;
}

// LERP -------------------------------------------------------------------------------------------

static constexpr inline float LERP_SMOOTHNESS = 20.0f;
static constexpr inline float SCREEN_FADE_DURATION = 0.2f;
static constexpr inline float ACC_LERP_FACTOR = LERP_SMOOTHNESS * 0.5f;
static constexpr inline float ROTATION_LERP = LERP_SMOOTHNESS * 2.0f;

inline float lerpFactor(const float& deltaTime, const float& base_smoothness)
{
	return 1.0f - std::exp(-base_smoothness * deltaTime);
}

inline float lerp(const float& a, const float& b, float factor)
{
	clamp(factor, 0.0f, 1.0f);

	return a + factor * (b - a);
}

inline int lerp(const int& a, const int& b, float factor)
{
	clamp(factor, 0.0f, 1.0f);

	float res = a + factor * (b - a);

	return static_cast<int>(std::round(res));
}

inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float factor)
{
	clamp(factor, 0.0f, 1.0f);

	return a + factor * (b - a);
}

inline sf::Color lerp(const sf::Color& currColor, const sf::Color& targetColor, float factor)
{
	clamp(factor, 0.0f, 1.0f);

	sf::Color colorDiff = targetColor - currColor;

	sf::Color newColor = {
		static_cast<sf::Uint8>(lerp(currColor.r, targetColor.r, factor)),
		static_cast<sf::Uint8>(lerp(currColor.g, targetColor.g, factor)),
		static_cast<sf::Uint8>(lerp(currColor.b, targetColor.b, factor)),
		static_cast<sf::Uint8>(lerp(currColor.a, targetColor.a, factor))
	};

	return newColor;
}


// EASING -------------------------------------------------------------------------------------
// 

using EasingFunction = float(*)(float t, float b, float c, float d);

namespace easing
{
	// Rewritten to fit the (t, b, c, d) format: t = current time, b = start value, c = change in value, d = duration
	inline float smoothstep(float t, float b, float c, float d)
	{
		if (d == 0.0f) return b; // avoid divide by zero

		// Normalize time to [0, 1]
		float x = std::clamp(t / d, 0.0f, 1.0f);

		// Apply smoothstep formula
		float smoothed = x * x * (3.0f - 2.0f * x);
		return b + c * smoothed;
	}

	inline float smootherstep(float t, float b, float c, float d)
	{
		if (d == 0.0f) return b; // avoid divide by zero

		// Normalize time to [0, 1]
		float x = std::clamp(t / d, 0.0f, 1.0f);

		// Apply smootherstep formula: 6x⁵ - 15x⁴ + 10x³
		float smoothed = x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f);
		return b + c * smoothed;
	}


	namespace quad
	{

		inline float in(float t, float b, float c, float d)
		{
			t /= d;
			return c * t * t + b;
		}

		inline float out(float t, float b, float c, float d)
		{
			t /= d;
			return -c * t * (t - 2.0f) + b;
		}

		inline float inOut(float t, float b, float c, float d)
		{
			t /= d / 2.0f;
			if (t < 1.0f) return c / 2.0f * t * t + b;
			t -= 1.0f;
			return -c / 2.0f * (t * (t - 2.0f) - 1.0f) + b;
		}
	}

	namespace back
	{
		inline static constexpr float DEFAULT_S = 1.70158f;

		inline float in(float t, float b, float c, float d)
		{
			const float s = DEFAULT_S;
			t /= d;
			return c * t * t * ((s + 1.0f) * t - s) + b;
		}

		inline float out(float t, float b, float c, float d)
		{
			const float s = DEFAULT_S;
			t = t / d - 1.0f;
			return c * (t * t * ((s + 1.0f) * t + s) + 1.0f) + b;
		}

		inline float inOut(float t, float b, float c, float d)
		{
			float s = DEFAULT_S * 1.525f;
			t /= d / 2.0f;
			if (t < 1.0f)
				return c / 2.0f * (t * t * ((s + 1.0f) * t - s)) + b;
			t -= 2.0f;
			return c / 2.0f * (t * t * ((s + 1.0f) * t + s) + 2.0f) + b;
		}
	}

	namespace expo
	{
		// Exponential ease-in
		inline float in(float t, float b, float c, float d)
		{
			return (t == 0) ? b : c * static_cast<float>(std::pow(2, 10 * (t / d - 1))) + b;
		}

		// Exponential ease-out
		inline float out(float t, float b, float c, float d)
		{
			return (t == d) ? b + c : c * (-static_cast<float>(std::pow(2, -10 * t / d)) + 1) + b;
		}

		// Exponential ease-in-out
		inline float inOut(float t, float b, float c, float d)
		{
			if (t == 0) return b;
			if (t == d) return b + c;
			t /= d / 2;
			if (t < 1) return c / 2 * static_cast<float>(std::pow(2, 10 * (t - 1))) + b;
			t--;
			return c / 2 * (-static_cast<float>(std::pow(2, -10 * t)) + 2) + b;
		}
	}

	namespace cubic
	{
		// Cubic ease-in
		inline float in(float t, float b, float c, float d)
		{
			t /= d;
			return c * t * t * t + b;
		}

		// Cubic ease-out
		inline float out(float t, float b, float c, float d)
		{
			t /= d;
			t--;
			return c * (t * t * t + 1) + b;
		}

		// Cubic ease-in-out
		inline float inOut(float t, float b, float c, float d)
		{
			t /= d / 2;
			if (t < 1) return c / 2 * t * t * t + b;
			t -= 2;
			return c / 2 * (t * t * t + 2) + b;
		}
	}
}

// VECTOR2F MATH ----------------------------------------------------------------------------------
// Compatre Vector2f
inline bool operator==(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
	return (vec1.x == vec2.x && vec1.y == vec2.y);
}

// Vector2f * float
inline sf::Vector2f operator*(const sf::Vector2f& vec, float scalar)
{
	return sf::Vector2f(vec.x * scalar, vec.y * scalar);
}

// Vector2f * Vector2f
inline sf::Vector2f operator*(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
	return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y);
}

// Snapping a sf::Vector2f onto a grid of squares (sideLength is gridSize)
inline sf::Vector2f posToGrid(const sf::Vector2f& pos, const float& gridSize)
{
	sf::Vector2f snappedPos = { std::trunc(pos.x / gridSize), std::trunc(pos.y / gridSize) };
	snappedPos *= gridSize;
	return snappedPos;
}

// Function to get normalized velocity from rotation
inline sf::Vector2f dirFromRot(const float angle)
{
	double rad = angle * pi / 180.0f; // Convert to radians
	return sf::Vector2f(static_cast<float>(std::cos(rad)), static_cast<float>(std::sin(rad))); // Normalized direction
}

// COLOR
// Dividing colors by floats
inline sf::Color operator/(const sf::Color& color, float scalar)
{
	if (scalar == 0.0f) scalar = 1.0f;

	auto divide = [scalar](sf::Uint8 c) -> sf::Uint8 {
		return static_cast<sf::Uint8>(std::clamp(c / scalar, 0.0f, 255.0f));
		};

	return sf::Color(
		divide(color.r),
		divide(color.g),
		divide(color.b),
		divide(color.a)
	);
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

// STD::VECTOR ------------------------------------------------------------------------------------
// Checking if item in vector
template <typename T>
bool vectorContains(const std::vector<T>& vec, const T& elem)
{
	return (std::find(vec.begin(), vec.end(), elem) != vec.end());
}