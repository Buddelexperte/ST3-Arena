#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Shotgun.h"

class Shotgun : public Weapon
{
private:
	static constexpr float BASE_DAMAGE = 1.0f;
	static constexpr float COOLDOWN = 1.0f;

public:
	Shotgun()
		:
		Weapon(
			ItemInfo("Shotgun", "Basic weapon, spread pattern"),	// ItemInfo
			BASE_DAMAGE,											// Damage
			std::make_unique<PS_Shotgun>(BASE_DAMAGE),				// ProjectileSpawner
			std::make_unique<ValueBar>(COOLDOWN)					// Cooldown	
		)
	{
	}

	UseResult activate(const ItemUse& use) override
	{
		switch (use)
		{
		case ItemUse::ATTACK:
			if (Weapon::activate(use) < UseResult::SUCCESS)
			{
				break;
			}

			projSpawner->shoot();
			break;
		default:
			break;
		}

		return UseResult::FAILURE;
	}
};