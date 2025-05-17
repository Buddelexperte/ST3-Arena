#pragma once

#include "ItemBaseClasses.h"

class PUtil_Root : public Perk
{
	static const inline ItemInfo INFO = ItemInfo("Utility Root Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public:
	PUtil_Root() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Speed : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Speed Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public:
	PUtil_Speed() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Reload : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Reload Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public: 
	PUtil_Reload() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Scan : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Scan Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public:
	PUtil_Scan() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Scan2 : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Scan Upgrade Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public:
	PUtil_Scan2() : Perk(INFO, TRIGGERS) {}
};
