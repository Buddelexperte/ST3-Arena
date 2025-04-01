#pragma once

#include "Projectile.h"
#include "EntityManager.h"

class PS_Pistol : public ProjectileSpawner
{
private:
	// Static constexpr defaults for memory efficiency
	static constexpr float OFFSET_X = 0.0f;
	static constexpr float OFFSET_Y = 0.0f;

	static constexpr float SIZE_X = 40.0f;
	static constexpr float SIZE_Y = 10.0f;

	static constexpr float ROT_OFFSET = 0.0f;

	static constexpr float SPEED = 1500.0f;
	static const inline sf::Color COLOR = sf::Color( 255, 0, 0, 255 );

	static const inline RenderInfo baseInfo = {
		{OFFSET_X, OFFSET_Y}, // Default offset
		{SIZE_X, SIZE_Y}, // Default projectile size
		ROT_OFFSET, // Default rotation offset
		{SPEED, SPEED}, // Default projectile speed (velocity later multiplied with direction)
		COLOR // Default projectile Color
	};

public:
	PS_Pistol(const float& damage)
		:
		ProjectileSpawner(baseInfo, damage)
	{ }

	virtual void shoot() override
	{
		// Get the forward direction vector
		sf::Vector2f forwardDir = dirFromRot(getRotation());

		// Calculate the new spawn position with the offset
		sf::Vector2f spawnPosition = getPosition() + (forwardDir * SIM_ARM_LENGTH);

		// Create projectile information with the adjusted position
		RenderInfo projectileInfo = {
			.pos = spawnPosition + baseInfo.pos,  // Apply the offset
			.size = baseInfo.size,
			.rot = getRotation() + baseInfo.rot,
			.velocity = forwardDir * baseInfo.velocity,  // Keep velocity aligned with direction
			.color = baseInfo.color
		};

		SpawnInformation spawnInfo = {
			.renderInfo = projectileInfo,
			.damage = getDamage()
		};

		ProjectileSpawner::shoot(spawnInfo);
	}
};