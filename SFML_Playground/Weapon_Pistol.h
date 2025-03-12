#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Pistol.h"

// TODO: Make WeaponInterfaces for shooting, melee and hitboxes

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
		switch (Weapon::activate(use))
		{
		case UseResult::SUCCESS:
			if (use == ItemUse::ATTACK)
			{
				projSpawner->shoot();
			}
			break;
		default:
			break;
		}
		
		return UseResult::FAILURE;
	}
};