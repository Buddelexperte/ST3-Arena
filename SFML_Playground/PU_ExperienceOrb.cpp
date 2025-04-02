#pragma once

#include "PU_ExperienceOrb.h" // Own header file
#include "GameInstance.h"

void PU_ExperienceOrb::tick_move(const float& deltaTime)
{
    static constexpr float DELTA_TIME_CORRECTION = 10.0f;

    const sf::Vector2f playerPos = gameInstance().getPlayer()->getPosition();
    const sf::Vector2f pickupPos = getPosition();

    sf::Vector2f direction = playerPos - pickupPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize direction vector
    bool bDistanceNearZero = shouldZero(distance);
    if (bDistanceNearZero)
    {
        setVelocity(sf::Vector2f(0.0f, 0.0f));
        return;
    }

    direction /= distance;

    constexpr float MAGNETIC_RANGE = 200.0f;    // Start pulling from this distance
    constexpr float BASE_SPEED = 10.0f;         // Base movement speed
    constexpr float MAX_SPEED = 500.0f;         // Maximum pull speed
    constexpr float ACCELERATION_FACTOR = 20.0f; // Speed multiplier for acceleration
    
    sf::Vector2f newVelo = sf::Vector2f(0.0f, 0.0f);
    if (distance < MAGNETIC_RANGE)
    {
        // Calculate speed based on proximity (closer = faster)
        float speed = BASE_SPEED + (1.0f - (distance / MAGNETIC_RANGE)) * (MAX_SPEED - BASE_SPEED);
        speed *= ACCELERATION_FACTOR * deltaTime * DELTA_TIME_CORRECTION;

        newVelo = direction * speed;
    }

    newVelo = lerp(getVelocity(), newVelo, LERP_SMOOTHNESS);
    sf::Vector2f offset = getVelocity() * deltaTime;

    addPosition(offset);
    setVelocity(newVelo);
}


void PU_ExperienceOrb::onPickup()
{
    SaveGame::currentData.score = (SaveGame::currentData.score + 1);
}

void PU_ExperienceOrb::spawn(const SpawnInformation& spawnInfo)
{
    setRenderInfo(spawnInfo.renderInfo);

	resetTimeAlive();

    collisionBox.setPos(getPosition());
    collisionBox.setSize(getSize());
}

void PU_ExperienceOrb::tick(const float& deltaTime)
{
	updateTimeAlive(deltaTime);

    tick_move(deltaTime);

	sf::Color currColor = getColor();
	currColor.a = static_cast<sf::Uint8>(((sin(timeAlive) + 2.0f) * 0.33f) * 255.0f);

	setColor(currColor);
	
	static constexpr float ROTATION_SPEED = 100.0f;

	// Increase rotation speed over time
	setRotation(getRotation() + ROTATION_SPEED * deltaTime);
}

void PU_ExperienceOrb::releaseToPool()
{
    GenericPool<PU_ExperienceOrb>::instance().release(std::unique_ptr<PU_ExperienceOrb>(this));
}

void PU_ExperienceOrb::addPosition(const sf::Vector2f& delta)
{
    Pickup::addPosition(delta);

    collisionBox.setPos(collisionBox.getPos() + delta);
}
