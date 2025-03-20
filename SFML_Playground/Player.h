#pragma once

#include "WidgetBase.h"
#include "Collision.h"
#include "Health.h"
#include "Flashlight.h"
#include "Inventory.h"
#include "Entity.h"

// PLAYER -----------------------------------------------------------------------------------------

class Player : 
	public Entity,
	public InputWidget, // Is also IMovable
	public IHasCollision,
	public IHasHealth
{
private:

	// Collision
	static const inline sf::Vector2f HITBOX_SIZE = sf::Vector2f(80.0f, 80.0f);
	CollisionBox collisionBox;

	Flashlight flashlight;
	Inventory inventory;

	// Health bar
	ValueBar invincibility;
	ValueBar healthBar;
	ValueBar& getValueBar() override
		{ return healthBar; }

	// Animation
	sf::Sprite playerSprite;
	std::vector<sf::Texture> playerTextures = {};
	int currentFrame = 0;
	float animationAccu = 0.0f;
	float animationSpeed = 0.0f;

	using IMovable::setVelocity; // Make this function private

	void tick_move(const float&) override;
	void tick_animation(const float&);
protected:
	sf::Keyboard::Key onKeyPressed(sf::Event*) override;
	bool onMouseClickL(sf::Event*) override;
	bool onMouseClickR(sf::Event*) override;
	void onMouseDownL() override;
	bool onMouseReleaseL(sf::Event*) override;
	float onMouseScrolled(sf::Event*) override;
public:
	Player(InputWidget*);
	~Player();

	// Entity
	void spawn() override;
	void tick(const float&) override;

	bool handleEvent(sf::Event* eventRef) override;

	Inventory& getInventory() { return inventory; }
	Flashlight& getFlashlight() { return flashlight; }

	// Movable Interface
	void setRenderInfo(const RenderInfo&) override;
	void setPosition(const sf::Vector2f&) override;
	void addPosition(const sf::Vector2f&) override;
	void setRotation(const float&) override;
	void setSize(const sf::Vector2f&) override;
	void setColor(const sf::Color&) override;

	// Collision
	Collidable* getCollision() override
		{ return &collisionBox; }
	
	void onCollision(IHasCollision* other) override;

	void collideWithEnemy(Enemy& enemy) override;
	void collideWithProjectile(Projectile& projectile) override;

	// Health
	void hurt(const float& delta) override;
};
