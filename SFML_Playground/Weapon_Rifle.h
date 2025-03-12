#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Rifle.h"

// TODO: FIX mouse being pressed down

class Rifle : public Weapon
{
private:
	const float cooldown = 2.0f;
public:
	Rifle()
		:
		Weapon(
			ItemInfo("Rifle", "Basic weapon, full automatic"),	// ItemInfo
			std::make_unique<PS_Rifle>(),						// ProjectileSpawner
			std::make_unique<ValueBar>(cooldown)				// Cooldown	
		)
	{}

	UseResult activate(const ItemUse& use) override
	{
		switch (use)
		{
		case ItemUse::ATTACK: case ItemUse::LOAD_UP:
			if (Weapon::activate(ItemUse::ATTACK) < UseResult::SUCCESS)
			{
				std::cout << "Weapon \"" << info.name << "\" failed to fire because of Cooldown" << std::endl;
				break;
			}
			projSpawner->shoot();
			return UseResult::SUCCESS;
			break;
		default:
			break;
		}

		return UseResult::FAILURE;
	}
};