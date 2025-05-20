#pragma once

#include "ItemBaseClasses.h"
#include "GameInstance.h"

class PSup_Root : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Support Root Perk", "Increase your max hp from 100-125hp");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float newMaxHp = 1.25f;

	void onEquip() override
	{
		gameInstance().getPlayer()->resetHealth(newMaxHp);
	}

public: 
	PSup_Root() : Perk(INFO, TRIGGERS) {}
};

class PSup_HealAura : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Heal Aura Perk");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {PerkTrigger::OnInterval};

	static constexpr float healAmount = 0.1f; // 10 hp
	static constexpr float healDelay = 2.0f; // 2 seconds
	ValueBar healTimer;

	void onInterval(const float& deltaTime) override
	{
		healTimer.addValue(-deltaTime);

		if (healTimer.isEmpty())
		{
			healTimer.fill_to_max();

			gameInstance().getPlayer()->heal(healAmount);
		}
	}

public: 
	PSup_HealAura() : Perk(INFO, TRIGGERS), healTimer(healDelay) {}
};

class PSup_Vampire : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Vampire Perk", "Receive 20% of an enemies health upon killing it");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {PerkTrigger::OnEnemyKilled};

	static constexpr float lifeFraction = 0.2f;

	void onEnemyKilled(PerkTriggerInfo& triggerInfo) override
	{
		Entity* entity = triggerInfo.actor;
		if (entity != nullptr)
		{
			IHasHealth* healthEntity = dynamic_cast<IHasHealth*>(entity);
			float healValue = healthEntity->getMaxHealth() * lifeFraction;
			gameInstance().getPlayer()->heal(healValue);
		}
	}

public: 
	PSup_Vampire() : Perk(INFO, TRIGGERS) {}
};

class PSup_DoubleLife : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Advanced Revive Perk", "Increase your hp to double the original value!");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float NEW_MAX_HP = 2.0f; // 200 hp

	void onEquip() override
	{
		Player* player = gameInstance().getPlayer();
		player->resetHealth(NEW_MAX_HP);
	}

public: 
	PSup_DoubleLife() : Perk(INFO, TRIGGERS) {}
};