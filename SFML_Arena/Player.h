#pragma once

#include "Collision.h"
#include "Health.h"
#include "W_Hud.h"
#include "Flashlight.h"
#include "DrawableShape.h"
#include "Inventory.h"
#include "Input.h"
#include "Entity.h"

#include "C_ColorFade.h"

// PLAYER -----------------------------------------------------------------------------------------

class Player : 
	public Entity,
	public IHasHealth,
	public IDrawableShapes,
	public IHasInput
{
private:
	// Collision
	static const inline sf::Vector2f HITBOX_SIZE = sf::Vector2f(80.0f, 80.0f);
	CollisionBox collisionBox;

	W_Hud hud;
	Flashlight flashlight;
	Inventory inventory;

	// Animation
	sf::Sprite playerSprite;
	std::vector<sf::Texture> playerTextures = {};
	int currentFrame = 0;
	float animationAccu = 0.0f;
	float animationSpeed = 0.0f;

	using IMovable::setVelocity; // Make this function private

	void tick_flashlight(const float&);
	void tick_gameplay(const float&);
	void tick_move(const float&) override;
	void tick_health(const float&) override;
	void tick_animation(const float&);
	void tick_color(const float&);

	ColorFade playerFade;
	static const inline sf::Color SHIELD_COLOR = sf::Color(100, 112, 255);

protected:
	sf::Keyboard::Key onKeyPressed(sf::Event*) override;
	bool onMouseClickL(sf::Event*) override;
	bool onMouseClickR(sf::Event*) override;
	void onMouseDownL() override;
	bool onMouseReleaseL(sf::Event*) override;
	float onMouseScrolled(sf::Event*) override;
public:
	Player();
	~Player() = default;

	// Entity
	void spawn( SpawnInformation) override;
	void spawn();
	void tick(const float&) override;
	void releaseToPool() override;

	bool handleEvent(sf::Event* eventRef) override;

	Inventory& getInventory() { return inventory; }
	Flashlight& getFlashlight() { return flashlight; }

	// Movable Interface
	void setRenderInfo(const RenderInfo&) override;
	void setPosition(const sf::Vector2f&) override;
	void addPosition(const sf::Vector2f&, const bool& bVelocityBased = true) override;
	void setRotation(const float&) override;
	void setSize(const sf::Vector2f&) override;
	void setColor(const sf::Color&) override;

	// Collision
	Collidable* getCollision() override
		{ return &collisionBox; }
	
	void onCollision(IHasCollision* other) override;

	void collideWithEnemy(Enemy& enemy) override;
	void collideWithProjectile(Projectile& projectile) override;

	void activateShield();
	void breakShield();

	// Health
	void resetHealth(const float& newMax)
	{
		IHasHealth::resetHealth(newMax);
		hud.reset_health();
	}

	void resetHealth() override
	{
		IHasHealth::resetHealth();
		hud.reset_health();
	}

	void hurt(const float& delta) override;
	void heal(const float& delta) override;

	W_Hud& getHud()
	{
		return hud;
	}
};
