#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Rifle.h"

class Rifle : public Weapon
{
private:
	static constexpr float damage = 0.1f;
public:
	Rifle()
		:
		Weapon(
			ItemInfo("Rifle", "Basic weapon, full automatic"),	// ItemInfo
			damage,
			std::make_unique<PS_Rifle>(getDamage()),			// ProjectileSpawner
			std::make_unique<ValueBar>(0.2f)					// Cooldown	
		)
	{}

	UseResult activate(const ItemUse& use) override
	{
		switch (use)
		{
		case ItemUse::ATTACK: case ItemUse::LOAD_UP:
			if (Weapon::activate(ItemUse::ATTACK) < UseResult::SUCCESS)
			{
				return UseResult::FAILURE_COOLDOWN;
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