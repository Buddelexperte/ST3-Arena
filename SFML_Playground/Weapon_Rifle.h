#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Rifle.h"

class Rifle : public Weapon
{
private:
	static constexpr float BASE_DAMAGE = 1.0f;
	static constexpr float COOLDOWN = 0.2f;

public:
	Rifle()
		:
		Weapon(
			ItemInfo("Rifle", "Basic weapon, full automatic"),		// ItemInfo
			BASE_DAMAGE,											// Damage		
			std::make_unique<PS_Rifle>(BASE_DAMAGE),				// ProjectileSpawner
			std::make_unique<ValueBar>(COOLDOWN)					// Cooldown	
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