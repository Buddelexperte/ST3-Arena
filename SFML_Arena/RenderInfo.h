#pragma once
#include <SFML/Graphics.hpp>

class IMovable  // Interface for movable entities
{
public:
	struct RenderInfo {
		sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f size = sf::Vector2f(100.0f, 100.0f);
		float rot = 0;
		sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
		sf::Color color = sf::Color::White;

		bool operator==(const RenderInfo& other) const {
			return pos == other.pos &&
				size == other.size &&
				rot == other.rot &&
				velocity == other.velocity &&
				color == other.color;
		}
	};
private:
	RenderInfo renderInfo;
	virtual void tick_move(const float&)
		{}
public:

	// RenderInfo
	virtual void setRenderInfo(const RenderInfo& newRenderInfo)
		{ renderInfo = newRenderInfo; }

	virtual RenderInfo getRenderInfo() const
		{ return renderInfo; }

	// Position
	virtual void setPosition(const sf::Vector2f& newPos)
		{ renderInfo.pos = newPos; }

	virtual void addPosition(const sf::Vector2f& deltaPos, const bool& bVelocityBased = true)
		{ renderInfo.pos += deltaPos; }

	virtual sf::Vector2f getPosition() const
		{ return renderInfo.pos; }

	// Size
	virtual void setSize(const sf::Vector2f& newSize)
		{ renderInfo.size = newSize; }

	virtual sf::Vector2f getSize() const
		{ return renderInfo.size; }

	// Rotation
	virtual void setRotation(const float& newRot)
		{ renderInfo.rot = newRot; }

	virtual float getRotation() const
		{ return renderInfo.rot; }

	// Color
	virtual void setColor(const sf::Color& newColor)
		{ renderInfo.color = newColor; }

	virtual sf::Color getColor() const
		{ return renderInfo.color; }

	// Velocity
	virtual void setVelocity(const sf::Vector2f& newVelo)
		{ renderInfo.velocity = newVelo; }

	virtual sf::Vector2f getVelocity() const
		{ return renderInfo.velocity; }
};