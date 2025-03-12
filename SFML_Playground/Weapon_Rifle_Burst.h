#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Pistol.h"

class Rifle_Burst : public Weapon
{
private:
	ValueBar burstCooldown = ValueBar(0.1f);
	const int burstSize = 3;
	int burstLeft = 0;
public:
	Rifle_Burst()
		:
		Weapon(
			ItemInfo("Pistol", "Basic weapon, semi automatic"),		// ItemInfo
			std::make_unique<PS_Pistol>(),							// ProjectileSpawner
			std::make_unique<ValueBar>(0.8f)						// Cooldown	
		)
	{
	}

	UseResult activate(const ItemUse& use) override
	{
		switch (use)
		{
		case ItemUse::NO_USE:
			if (burstLeft > 0)
			{
				if (burstCooldown.isNotEmpty())
				{
					//std::cout << "Failed, burst cooldown was not empty" << std::endl;
					return UseResult::FAILURE_COOLDOWN;
				}
				burstCooldown.reset();
				projSpawner->shoot();
				burstLeft--;
				return UseResult::SUCCESS;
			}

			break;
		case ItemUse::ATTACK:
			if (Weapon::activate(use) < UseResult::SUCCESS)
			{
				break;
			}

			projSpawner->shoot();
			burstLeft = burstSize - 1;
			return UseResult::SUCCESS;

			break;
		default:
			break;
		}

		return UseResult::FAILURE;
	}

	void tick(const float& deltaTime) override
	{
		if (burstLeft > 0)
		{
			float cooldownDelta = -deltaTime;
			burstCooldown.addValue(cooldownDelta);
		}

		// Activating NO_USE (Burst)
		Weapon::tick(deltaTime);
	}
};