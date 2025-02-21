#pragma once

#include "BaseClasses.h"
#include "Collision.h"

// PLAYER -----------------------------------------------------------------------------------------

class Player : public InputWidget, public ICollidable
{
private:
	sf::Sprite playerSprite;

	CollisionBox collisionBox;

	RenderInfo renderInfo;
	sf::Vector2f direction = { 0.0f, 0.0f };

	std::vector<sf::Texture> playerTextures = {};
	int currentFrame = 0;
	float animationAccu = 0.0f;
	float animationSpeed = 0.0f;

	void tick_collision(const float& deltaTime) override;
	void calcMovement(const float&);
protected:
	sf::Keyboard::Key keyboardInput(sf::Event*) override;
	sf::Mouse::Button mouseInput(sf::Event*) override;
	float scrollInput(sf::Event*) override;
public:
	Player(InputWidget*);
	void tick(const float&) override;
	void setPos(const sf::Vector2f&) override;
	void addPos(const sf::Vector2f&) override;
	sf::Vector2f getPos() const override;
	void setRot(const float&) override;
	float getRot() const override;
	void setSize(const sf::Vector2f&) override;
	sf::Vector2f getVelocity() const { return renderInfo.velocity; };
	sf::Vector2f getDirection() const { return direction; };

	RenderInfo getRenderInfo() const { return renderInfo; }
	// Collision-Interface
	ICollidable* getCollision() override { return &collisionBox; }
	void onCollision(ICollidable* other) override;
};
