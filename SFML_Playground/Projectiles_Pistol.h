#pragma once

#include "ProjectileManager.h"

class PS_Pistol : public ProjectileSpawner
{
private:
	const RenderInfo baseInfo = {
		{0.0f, 0.0f}, // Default offset
		{40.0f, 10.0f}, // Default projectile size
		0.0f, // Default rotation offset
		{1500.0f, 1500.0f}, // Default projectile speed (velocity later multiplied with direction)
		sf::Color::Red // Default projectile Color
	};

public:
	PS_Pistol(const float& damage)
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

		manager->shootProjectile(projectileInfo, getDamage());
	}
};