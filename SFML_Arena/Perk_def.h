#pragma once

#include "ItemBaseClasses.h"

class PDef_Root : public Perk {
private:
	static const inline ItemInfo INFO = ItemInfo("Defense Root Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public:
	PDef_Root()
		: Perk(INFO, TRIGGERS)
	{
	}
};
