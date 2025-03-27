#pragma once

#include "Pickup.h" // Own header
#include "GameInstance.h"

// TODO: Move the weird experience orb logic into own class

void Pickup::tick_move(const float& deltaTime)
{
	static constexpr float ROTATION_MULTIPLIER = 1000.0f;
	// TODO: Add magnet towards player

	// Normalize lifetime to scale correctly (assuming lifeTime goes from 1.0 to 0.0)
	float normalizedLifetime = lifeTime.getPercentage(); // Ensure this is clamped between 0.0 and 1.0
	// Lerp between original size and zero based on normalized lifetime
	sf::Vector2f newSize = lerp(startSize, sf::Vector2f(0.0f, 0.0f), 1.0f - normalizedLifetime);

	setSize(newSize);

	// Increase rotation speed over time
	rotationSpeed += deltaTime * ROTATION_MULTIPLIER; // Adjust multiplier for desired acceleration
	setRotation(getRotation() + rotationSpeed * deltaTime);
}

void Pickup::tick_lifetime(const float& deltaTime)
{
	lifeTime.addValue(-deltaTime);

	if (lifeTime.isEmpty())
	{
		kill_self();
	}
}


Pickup::Pickup()
	: Entity(EntityType::Pickup),
	collisionBox(this, getPosition(), getSize())
{

}

void Pickup::setValue(const float& newVal)
{
	value = newVal;
}

float Pickup::getValue() const
{
	return value;
}

void Pickup::spawn(const SpawnInformation& spawnInfo)
{
	rotationSpeed = 0.0f;
	setRenderInfo(spawnInfo.renderInfo);
	startSize = getRenderInfo().size;
	resetHealth();
	setValue(spawnInfo.health); // Health is used for value flag
	lifeTime.reset();
}

void Pickup::tick(const float& deltaTime)
{
	tick_lifetime(deltaTime);

	tick_move(deltaTime);
}

void Pickup::releaseToPool()
{
	GenericPool<Pickup>::instance().release(std::unique_ptr<Pickup>(this));
}

void Pickup::onCollision(IHasCollision* other)
{
	other->collideWithPickup(*this);
}

void Pickup::setSize(const sf::Vector2f& newSize)
{
	Entity::setSize(newSize);
	collisionBox.setSize(newSize);
}
