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
	{ }

};

class PDef_Parry : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Defense Parry Perk", "50% chance to block hits for 3s after receiving damage");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {PerkTrigger::OnEnemyContact, PerkTrigger::OnInterval};

	// CONFIGURATION
	static constexpr float parryChance = 0.5f;
	static constexpr float parryCooldownTime = 2.0f;

	// STATE
	ValueBar parryCooldown;
	bool bParryAvailable = true;

	// CONTACT: When enemy tries to deal contact damage
	void onEnemyContact(PerkTriggerInfo& triggerInfo) override
	{
		if (!bParryAvailable)
			return;

		if (!RNG::chance(parryChance))
		{
			startParryCooldown(); // Reset parry cooldown
			std::cout << "No parry this time" << std::endl;
			return;
		}

		startParryWindow();
	}

	// TICK: Called every frame
	void onInterval(const float& deltaTime) override
	{
		if (!bParryAvailable)
		{
			parryCooldown.addValue(-deltaTime);

			if (parryCooldown.isEmpty())
			{
				bParryAvailable = true;
				std::cout << "Parry ready again." << std::endl;
			}
		}
	}

	// Parry helper functions

	void startParryCooldown()
	{
		bParryAvailable = false;
		parryCooldown.fill_to_max(); // Reset parry cooldown
	}

	void startParryWindow()
	{
		startParryCooldown();
		gameInstance().getInventory().fillHurtFreq(); // Apply invuln as if been hit
		std::cout << "PARRY! (for " << gameInstance().getInventory().getHurtFreq() << " seconds)" << std::endl;
	}

public:
	PDef_Parry()
		: Perk(INFO, TRIGGERS), parryCooldown(parryCooldownTime)
	{ }

};

class PDef_Shield : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Passive Shield Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = { PerkTrigger::OnInterval, PerkTrigger::OnEnemyContact };

	static constexpr float shieldRegenTime = 5.0f;
	ValueBar shieldTimer;
	bool bShieldActive = false;

	void onInterval(const float& deltaTime) override
	{
		if (bShieldActive)
			return;

		shieldTimer.addValue(-deltaTime);

		if (shieldTimer.isEmpty())
		{
			shieldTimer.fill_to_max();

			bShieldActive = true;
			gameInstance().getPlayer()->activateShield();
		}
	}

	void onEnemyContact(PerkTriggerInfo& triggerInfo) override
	{
		// Break shield if it was active to allow damage to come in again
		if (bShieldActive)
		{
			bShieldActive = false;
			gameInstance().getPlayer()->breakShield();
		}

		// If shield was not already active, reset timer for activation 
		shieldTimer.fill_to_max();
	}

public:
	PDef_Shield() : Perk(INFO, TRIGGERS), shieldTimer(shieldRegenTime) {}
};