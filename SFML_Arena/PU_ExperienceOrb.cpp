#pragma once

#include "PU_ExperienceOrb.h" // Own header file
#include "GameInstance.h"

void PU_ExperienceOrb::tick_move(const float& deltaTime)
{

	Player* playerRef = gameInstance().getPlayer();
    const sf::Vector2f playerPos = playerRef->getPosition();
    const sf::Vector2f pickupPos = getPosition();

    sf::Vector2f direction = playerPos - pickupPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize direction vector
    const bool bDistanceNearZero = shouldZero(distance);
    if (bDistanceNearZero)
    {
        setVelocity(sf::Vector2f(0.0f, 0.0f));
        return;
    }

    direction /= distance;

    const float MAGNETIC_RANGE = playerRef->getInventory().getMagneticRange();  // Start pulling from this distance
    static constexpr float BASE_SPEED = 10.0f;                                  // Base movement speed
    static constexpr float MAX_SPEED = 700.0f;                                  // Maximum pull speed
    static constexpr float ACCELERATION_FACTOR = 10.0f;                         // Speed multiplier for acceleration
    
    sf::Vector2f newVelo = sf::Vector2f(0.0f, 0.0f);
    if (distance < MAGNETIC_RANGE)
    {
        // Calculate speed based on proximity (closer = faster)
        float speed = BASE_SPEED + (1.0f - (distance / MAGNETIC_RANGE)) * (MAX_SPEED - BASE_SPEED);
        speed *= ACCELERATION_FACTOR;

        newVelo = direction * speed;
    }

    float factor = lerpFactor(deltaTime, LERP_SMOOTHNESS);
    newVelo = lerp(getVelocity(), newVelo, factor);
    sf::Vector2f offset = getVelocity() * deltaTime;

    setVelocity(newVelo);
    addPosition(offset);
}


void PU_ExperienceOrb::onPickup()
{
    unsigned int expToAdd = getExp();
    gameInstance().getPlayer()->getInventory().getLevelSystem().addPoints(expToAdd);
}

void PU_ExperienceOrb::spawn(SpawnInformation spawnInfo)
{
    experienceValue = static_cast<unsigned int>(spawnInfo.damage);

    spawnInfo.renderInfo.color = getCorrectColor(experienceValue);
    spawnInfo.renderInfo.size = getCorrectSize(experienceValue);

    setRenderInfo(spawnInfo.renderInfo);


	resetTimeAlive();

    collisionBox.setPos(getPosition());
    collisionBox.setSize(getSize());
}

sf::Color PU_ExperienceOrb::getCorrectColor(const unsigned int& experienceGroup) const
{
    return sf::Color::Yellow;

    switch (experienceGroup)
    {
    case 1:
        return sf::Color::Green;
        break;
    case 2:
        return sf::Color::Blue;
        break;
    case 5:
        return sf::Color::Cyan;
        break;
    case 10:
        return sf::Color::Yellow;
        break;
    default:
        break;
    }

	// Default color if no match is found
    return sf::Color::White;
}

sf::Vector2f PU_ExperienceOrb::getCorrectSize(const unsigned int& experienceGroup) const
{
    switch (experienceGroup)
    {
    case 1:
        return sf::Vector2f(10.0f, 10.0f);
        break;
    case 2:
        return sf::Vector2f(20.0f, 20.0f);
        break;
    case 5:
        return sf::Vector2f(30.0f, 30.0f);
        break;
    case 10:
        return sf::Vector2f(50.0f, 50.0f);
        break;
    default:
        break;
    }

	// Default size if no match is found
    return sf::Vector2f(30.0f, 30.0f);
}

void PU_ExperienceOrb::tick(const float& deltaTime)
{
	updateTimeAlive(deltaTime);

    tick_move(deltaTime);

	// Update alpha based on time alive
	sf::Color currColor = getColor();
	currColor.a = static_cast<sf::Uint8>(((sin(timeAlive) + 2.0f) * 0.33f) * 255.0f);
	setColor(currColor);
	
	// Increase rotation speed over time
	static constexpr float ROTATION_SPEED = 100.0f;
	setRotation(getRotation() + ROTATION_SPEED * deltaTime);
}

void PU_ExperienceOrb::releaseToPool()
{
    GenericPool<PU_ExperienceOrb>::instance().release(std::unique_ptr<PU_ExperienceOrb>(this));
}

void PU_ExperienceOrb::addPosition(const sf::Vector2f& delta, const bool& bVelocityBased)
{
    Pickup::addPosition(delta, bVelocityBased);

    collisionBox.setPos(collisionBox.getPos() + delta);
}

unsigned int PU_ExperienceOrb::getExp() const
{
    return experienceValue;
}

void PU_ExperienceOrb::setRenderInfo(const IMovable::RenderInfo& newRenderInfo)
{
    Pickup::setRenderInfo(newRenderInfo);

    collisionBox.setPos(getPosition());
    collisionBox.setSize(getSize());
}

void PU_ExperienceOrb::setPosition(const sf::Vector2f& newPos)
{
    Pickup::setPosition(newPos);

    collisionBox.setPos(newPos);
}

void PU_ExperienceOrb::setSize(const sf::Vector2f& newSize)
{
    Pickup::setSize(newSize);

    collisionBox.setSize(newSize);
}
