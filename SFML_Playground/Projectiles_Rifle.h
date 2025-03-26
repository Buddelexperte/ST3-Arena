#pragma once

#include "Projectile.h"
#include "EntityManager.h"

class PS_Rifle : public ProjectileSpawner
{
private:
	// Static constexpr defaults for memory efficiency
	static constexpr float OFFSET_X = 0.0f;
	static constexpr float OFFSET_Y = 0.0f;

	static constexpr float SIZE_X = 20.0f;
	static constexpr float SIZE_Y = 10.0f;

	static constexpr float ROT_OFFSET = 0.0f;

	static constexpr float SPEED = 1500.0f;
	static const inline sf::Color COLOR = sf::Color( 255, 0, 0, 255);

	static const inline RenderInfo baseInfo = {
		{OFFSET_X, OFFSET_Y}, // Default offset
		{SIZE_X, SIZE_Y}, // Default projectile size
		ROT_OFFSET, // Default rotation offset
		{SPEED, SPEED}, // Default projectile speed (velocity later multiplied with direction)
		COLOR // Default projectile Color
	};

public:
	PS_Rifle(const float& damage)
		:
		ProjectileSpawner(baseInfo, damage)
	{ }

	virtual void shoot() override
	{
		RenderInfo projectileInfo = {
			projectileInfo.pos = getPosition() + baseInfo.pos,
			projectileInfo.size = baseInfo.size,
			projectileInfo.rot = getRotation() + baseInfo.rot,
			projectileInfo.velocity = dirFromRot(getRotation()) * baseInfo.velocity,
			projectileInfo.color = baseInfo.color
		};
		
		SpawnInformation spawnInfo = {
			.renderInfo = projectileInfo,
			.damage = getDamage()
		};

		EntityManager::getInstance().spawnEntity<Projectile>(spawnInfo);
	}

};