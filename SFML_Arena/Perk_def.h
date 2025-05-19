#pragma once

#include "ItemBaseClasses.h"
#include "GameInstance.h"

class PDef_Root : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Defense Root Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float newDamageMulti = 0.75f;

	void onEquip() override
	{
		gameInstance().getInventory().applyHurtMultiplier(newDamageMulti);
	}
public:
	PDef_Root()
		: Perk(INFO, TRIGGERS)
	{
	}

};
