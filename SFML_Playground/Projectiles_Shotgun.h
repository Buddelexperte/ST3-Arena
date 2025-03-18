#pragma once

#include "ProjectileManager.h"

class PS_Shotgun : public ProjectileSpawner
{
private:
	// Shotgun pattern
	static constexpr int NUM_SHELLS = 7;
	static constexpr float SPREAD_DEGREES_TOTAL = 30.0;
	static constexpr float ANGLE_PER_SHELL = SPREAD_DEGREES_TOTAL / NUM_SHELLS;
	static constexpr float OFFSET_VARIETY = 50.0f;

	// Static constexpr defaults for memory efficiency
	static constexpr float OFFSET_X = 0.0f;
	static constexpr float OFFSET_Y = 0.0f;

	static constexpr float SIZE_X = 10.0f;
	static constexpr float SIZE_Y = 10.0f;

	static constexpr float ROT_OFFSET = 0.0f;

	static constexpr float SPEED = 1500.0f;
	static const inline sf::Color COLOR = sf::Color(255, 0, 0, 255);

	static const inline RenderInfo baseInfo = {
		{OFFSET_X, OFFSET_Y}, // Default offset
		{SIZE_X, SIZE_Y}, // Default projectile size
		ROT_OFFSET, // Default rotation offset
		{SPEED, SPEED}, // Default projectile speed (velocity later multiplied with direction)
		COLOR // Default projectile Color
	};

public:
	PS_Shotgun(const float& damage)
		:
		ProjectileSpawner(baseInfo, damage)
	{
	}

	virtual void shoot() override
	{
		const float base_rotation = getRotation() - (SPREAD_DEGREES_TOTAL / 2.0f) + ANGLE_PER_SHELL;
		const sf::Vector2f dir_offset = dirFromRot(getRotation()) * OFFSET_VARIETY;

		for (int i = 0; i < NUM_SHELLS; i++)
		{
			float shell_rotation = base_rotation + (i * ANGLE_PER_SHELL);
			sf::Vector2f offset = (i % 2 == 0) ? sf::Vector2f(0.f, 0.f) : dir_offset;

			RenderInfo projectileInfo{
				projectileInfo.pos = getPosition() + baseInfo.pos + offset,
				projectileInfo.size = baseInfo.size,
				projectileInfo.rot = shell_rotation + baseInfo.rot,
				projectileInfo.velocity = dirFromRot(shell_rotation) * baseInfo.velocity,
				projectileInfo.color = baseInfo.color
			};

			manager->createProjectile(projectileInfo, getDamage());
		}
	}

};