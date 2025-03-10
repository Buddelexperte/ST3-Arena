#pragma once

#include "ItemBaseClasses.h"

// TODO: Make WeaponInterfaces for shooting, melee and hitboxes

class Pistol : public Weapon
{
public:
	Pistol()
		: Weapon(ItemInfo("Pistol", "Basic weapon, semi automatic"))
	{}

	UseResult activate(ItemUse use) override
	{
		Weapon::activate(use);
		if (use == ItemUse::ATTACK)
			projSpawner.spawnProjectile();
		return UseResult::SUCCESS;
	}
};