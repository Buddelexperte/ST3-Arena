#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Pistol.h"

class Pistol : public Weapon
{
public:
	Pistol()
		: 
		Weapon(
			ItemInfo("Pistol", "Basic weapon, semi automatic"),		// ItemInfo
			std::make_unique<PS_Pistol>(),							// ProjectileSpawner
			std::make_unique<ValueBar>(0.2f)						// Cooldown	
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

	virtual float getDamage() const
	{
		return 0.2;
	}
};