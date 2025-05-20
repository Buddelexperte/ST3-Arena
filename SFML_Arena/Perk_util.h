#pragma once

#include "ItemBaseClasses.h"
#include "GameInstance.h"

class PUtil_Root : public Perk
{
	static const inline ItemInfo INFO = ItemInfo("Utility Root Perk", "Increases time between hits taken to 1 second");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float NEW_COOLDOWN_MULTI = 2.0f;

	void onEquip() override
	{
		gameInstance().getInventory().setCooldownMultiplier(NEW_COOLDOWN_MULTI);
	}

public:
	PUtil_Root() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Speed : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Speed Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float NEW_SPEED_MULTI = 2.0f;

	void onEquip() override
	{
		gameInstance().getInventory().setSpeedMultiplier(NEW_SPEED_MULTI);
	}

public:
	PUtil_Speed() : Perk(INFO, TRIGGERS) {}
};

class PUtil_FurtherLight : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Reload Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float ADDED_FLASHLIGHT_RADIUS = 200.0f;

	void onEquip() override
	{
		Flashlight& flashlight = gameInstance().getFlashlight();
		float currRadius = flashlight.getRadius();
		flashlight.setRadius(currRadius + ADDED_FLASHLIGHT_RADIUS);
	}

public: 
	PUtil_FurtherLight() : Perk(INFO, TRIGGERS) {}
};

class PUtil_DoubleCone : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Reload Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr bool USE_DOUBLE_CONE = true;

	void onEquip() override
	{
		Flashlight& flashlight = gameInstance().getFlashlight();
		flashlight.setSecondCone(USE_DOUBLE_CONE);
	}

public:
	PUtil_DoubleCone() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Magnet : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Scan Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float ADDED_MAGNET_RANGE = 200.0f;

	void onEquip() override
	{
		gameInstance().getInventory().addMagneticRange(ADDED_MAGNET_RANGE);
	}

public:
	PUtil_Magnet() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Magnet2 : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Scan Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float ADDED_MAGNET_RANGE = 500.0f;

	void onEquip() override
	{
		gameInstance().getInventory().addMagneticRange(ADDED_MAGNET_RANGE);
	}

public:
	PUtil_Magnet2() : Perk(INFO, TRIGGERS) {}
};

class PUtil_Scan : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Scan Upgrade Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

public:
	PUtil_Scan() : Perk(INFO, TRIGGERS) {}
};
