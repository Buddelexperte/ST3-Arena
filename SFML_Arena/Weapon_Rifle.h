#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Rifle.h"

class Rifle : public Weapon
{
private:
	// For constructing derived classes
	static constexpr float BASE_DAMAGE = 0.2f;
	static constexpr float BASE_COOLDOWN = 0.2f;

public:
	Rifle()
		: Weapon(
			ItemInfo("Rifle", "Basic weapon, full automatic"),
			BASE_DAMAGE,
			std::make_unique<PS_Rifle>(BASE_DAMAGE),
			std::make_unique<ValueBar>(BASE_COOLDOWN)
		)
	{ }

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