#pragma once

#include "P_Sparkle.h" // Own header
#include "GameInstance.h"

P_Sparkle::P_Sparkle()
	: Entity(EntityType::PARTICLE),
	IHasLifetime(0.0f),
	collisionBox(this, getPosition(), getSize())
{

}

void P_Sparkle::tick_move(const float& deltaTime)
{
	static constexpr float ROTATION_MULTIPLIER = 1000.0f;

	// Normalize lifetime to scale correctly (assuming lifeTime goes from 1.0 to 0.0)
	float normalizedLifetime = getLifetime().getPercentage(); // Ensure this is clamped between 0.0 and 1.0
	// Lerp between original size and zero based on normalized lifetime
	sf::Vector2f newSize = lerp(startSize, sf::Vector2f(0.0f, 0.0f), 1.0f - normalizedLifetime);

	setSize(newSize);

	// Increase rotation speed over time
	rotationSpeed += deltaTime * ROTATION_MULTIPLIER; // Adjust multiplier for desired acceleration
	setRotation(getRotation() + rotationSpeed * deltaTime);
}

void P_Sparkle::setValue(const float& newVal)
{
	value = newVal;
}

float P_Sparkle::getValue() const
{
	return value;
}

void P_Sparkle::spawn(const SpawnInformation& spawnInfo)
{
	resetLifetime(spawnInfo.health);
	setRenderInfo(spawnInfo.renderInfo);

	rotationSpeed = 0.0f;
	startSize = getRenderInfo().size;
}

void P_Sparkle::tick(const float& deltaTime)
{
	tick_lifetime(deltaTime);

	tick_move(deltaTime);
}

void P_Sparkle::releaseToPool()
{
	GenericPool<P_Sparkle>::instance().release(std::unique_ptr<P_Sparkle>(this));
}

void P_Sparkle::onCollision(IHasCollision* other)
{
	other->collideWithPickup(*this);
}

void P_Sparkle::setSize(const sf::Vector2f& newSize)
{
	Entity::setSize(newSize);
	collisionBox.setSize(newSize);
}
