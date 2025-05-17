#pragma once

#include "ItemBaseClasses.h"

class PSup_Root : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Support Root Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	PSup_Root() : Perk(INFO, TRIGGERS) {}
};

class PSup_HealAura : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Heal Aura Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	PSup_HealAura() : Perk(INFO, TRIGGERS) {}
};

class PSup_Ammo : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Ammo Regen Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	PSup_Ammo() : Perk(INFO, TRIGGERS) {}
};

class PSup_Revive : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Revive Ally Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	PSup_Revive() : Perk(INFO, TRIGGERS) {}
};

class PSup_Revive2 : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Advanced Revive Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	PSup_Revive2() : Perk(INFO, TRIGGERS) {}
};
