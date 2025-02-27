#pragma once

#include "BaseClasses.h"
#include "Collision.h"

// PLAYER -----------------------------------------------------------------------------------------

class Player : public InputWidget, public ICollidable
{
private:
	sf::Sprite playerSprite;

	CollisionBox collisionBox;

	sf::Vector2f direction = { 0.0f, 0.0f };

	std::vector<sf::Texture> playerTextures = {};
	int currentFrame = 0;
	float animationAccu = 0.0f;
	float animationSpeed = 0.0f;

	using IMovable::setVelocity; // Make this function private

	void tick_collision(const float& deltaTime) override;
	void calcMovement(const float&);
protected:
	sf::Keyboard::Key keyboardInput(sf::Event*) override;
	sf::Mouse::Button mouseInput(sf::Event*) override;
	float scrollInput(sf::Event*) override;
public:
	Player(InputWidget*);
	void tick(const float&) override;
	// Movable Interface
	void setPosition(const sf::Vector2f&) override;
	void addPosition(const sf::Vector2f&) override;
	void setRotation(const float&) override;
	void setSize(const sf::Vector2f&) override;
	sf::Vector2f getDirection() const { return direction; };
	// Collision-Interface
	ICollidable* getCollision() override { return &collisionBox; }
	void onCollision(ICollidable* other) override;
};
