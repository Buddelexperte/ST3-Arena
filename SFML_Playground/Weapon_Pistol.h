#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Pistol.h"

// TODO: Make WeaponInterfaces for shooting, melee and hitboxes

class Pistol : public Weapon
{
private:
	virtual float getMaxCooldown() const override
	{
		return 0.2f;
	}
public:
	Pistol()
		: 
		Weapon(
			ItemInfo("Pistol", "Basic weapon, semi automatic"),		// ItemInfo
			std::make_unique<PS_Pistol>()							// ProjectileSpawner
		)
	{}

	UseResult activate(const ItemUse& use) override
	{
		switch (Weapon::activate(use))
		{
		case UseResult::SUCCESS:
			if (use == ItemUse::ATTACK)
			{
				projSpawner->spawnProjectile();
			}
			break;
		default:
			break;
		}
		
		return UseResult::FAILURE;
	}
};