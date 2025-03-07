#pragma once

#include "BaseClasses.h"
#include "Collision.h"
#include "Flashlight.h"
#include "Inventory.h"

// PLAYER -----------------------------------------------------------------------------------------

class Player : public InputWidget, public ICollidable
{
private:
	sf::Sprite playerSprite;

	CollisionBox collisionBox;

	Flashlight flashlight;
	Inventory inventory;

	sf::Vector2f direction = { 0.0f, 0.0f };

	std::vector<sf::Texture> playerTextures = {};
	int currentFrame = 0;
	float animationAccu = 0.0f;
	float animationSpeed = 0.0f;

	using IMovable::setVelocity; // Make this function private

	void tick_collision(const float& deltaTime) override;
	void tick_move(const float&) override;
protected:
	sf::Keyboard::Key onKeyPressed(sf::Event*) override;
	bool onMouseClickL(sf::Event*) override;
	bool onMouseClickR(sf::Event*) override;
	bool onMouseReleaseL(sf::Event*) override;
	float onMouseScrolled(sf::Event*) override;
public:
	Player(InputWidget*);

	Player* spawn(const sf::Vector2f& = (sf::Vector2f(0.0f, 0.0f)));

	void tick(const float&) override;

	bool handleEvent(sf::Event* eventRef) override;

	Inventory& getInventory() { return inventory; }
	Flashlight& getFlashlight() { return flashlight; }

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
