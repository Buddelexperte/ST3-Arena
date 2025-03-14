#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Pistol.h"

class Pistol : public Weapon
{
private:
	static constexpr float BASE_DAMAGE = 0.2f;
	static constexpr float COOLDOWN = 0.2f;

public:
	Pistol()
		:
		Weapon(
			ItemInfo("Pistol", "Basic weapon, semi automatic"),		// ItemInfo
			BASE_DAMAGE,											// Damage
			std::make_unique<PS_Pistol>(BASE_DAMAGE),				// ProjectileSpawner
			std::make_unique<ValueBar>(COOLDOWN)					// Cooldown	
		)
	{}

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