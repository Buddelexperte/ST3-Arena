#pragma once

#include "ItemBaseClasses.h"

class POff_Root : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Offense Root Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	POff_Root() : Perk(INFO, TRIGGERS) {}
};

class POff_Crit : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Critical Strike Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	POff_Crit() : Perk(INFO, TRIGGERS) {}
};

class POff_DOT : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Damage over Time Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	POff_DOT() : Perk(INFO, TRIGGERS) {}
};

class POff_DOT_Upgrade : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("DOT Upgrade Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	POff_DOT_Upgrade() : Perk(INFO, TRIGGERS) {}
};
