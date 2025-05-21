#pragma once

#include "ItemBaseClasses.h"
#include "GameInstance.h"

class POff_Root : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Offense Root Perk", "-50% cooldown for your weapon");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float COOLDOWN_MULTIPLIER = 2.0f;

	void onEquip() override
	{
		gameInstance().getInventory().setCooldownMultiplier(COOLDOWN_MULTIPLIER);
	}

public: 
	POff_Root() : Perk(INFO, TRIGGERS) {}
};

class POff_Crit : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Critical Hit Perk", "20% chance to deal double the damage");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {};

	static constexpr float ADD_CRIT_CHANCE = 0.2f;

	void onEquip() override
	{
		gameInstance().getInventory().addCritChance(ADD_CRIT_CHANCE);
	}

public: 
	POff_Crit() : Perk(INFO, TRIGGERS) {}
};

class POff_DOT : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("Damage over Time Perk", "Deal 20hp damage to already hit targets every second");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {PerkTrigger::OnEnemyGotHit, PerkTrigger::OnInterval};

	static constexpr float dotInterval = 1.0f; // 1 second
	static constexpr float dotValue = 0.2f; // damage
	ValueBar dotTimer;

	std::unordered_set<IHasHealth*> bloodyEntities;

	void removeDeadTargets()
	{
		for (auto it = bloodyEntities.begin(); it != bloodyEntities.end();)
		{
			IHasHealth* entity = *it;

			if (!entity) {
				it = bloodyEntities.erase(it);
				continue;
			}

			if (entity->isDead())
			{
				it = bloodyEntities.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void onEnemyGotHit(PerkTriggerInfo& triggerInfo) override
	{
		if (triggerInfo.actor != nullptr)
		{
			IHasHealth* healthEntity = dynamic_cast<IHasHealth*>(triggerInfo.actor);
			if (healthEntity != nullptr)
			{
				bloodyEntities.insert(healthEntity);
			}
		}
	}

	void onInterval(const float& deltaTime) override
	{
		// Stop blood-ticking dead enemies to avoid false flagging as bloody on new spawns due to pointer recycling
		removeDeadTargets();

		dotTimer.addValue(-deltaTime);

		if (dotTimer.isEmpty())
		{
			dotTimer.fill_to_max();

			for (auto it = bloodyEntities.begin(); it != bloodyEntities.end();)
			{
				IHasHealth* entity = *it;

				if (!entity) {
					it = bloodyEntities.erase(it);
					continue;
				}

				entity->hurt(dotValue);

				if (entity->isDead())
				{
					it = bloodyEntities.erase(it);
				}
				else
				{
					++it;
				}
			}

		}
	}

public: 
	POff_DOT() : Perk(INFO, TRIGGERS), dotTimer(dotInterval) {}
};

class POff_DOT_Upgrade : public Perk
{
private:
	static const inline ItemInfo INFO = ItemInfo("DOT Upgrade Perk", "Deal additional 20hp damage to already hit targets every 0.5 seconds (stackable)");
	static const inline std::unordered_set<PerkTrigger> TRIGGERS = {PerkTrigger::OnEnemyGotHit, PerkTrigger::OnInterval};

	static constexpr float dotInterval = 0.3f; // 0.5 seconds
	static constexpr float dotValue = 0.2f; // damage
	ValueBar dotTimer;

	std::vector<IHasHealth*> bloodyEntities;

	void onEnemyGotHit(PerkTriggerInfo& triggerInfo) override
	{
		Entity* actor = triggerInfo.actor;
		if (actor)
		{
			IHasHealth* healthEntity = dynamic_cast<IHasHealth*>(actor);
			if (healthEntity)
			{
				bloodyEntities.push_back(healthEntity);
			}
		}
	}

	void onInterval(const float& deltaTime) override
	{
		dotTimer.addValue(-deltaTime);

		if (dotTimer.isEmpty())
		{
			dotTimer.fill_to_max();

			for (size_t i = 0; i < bloodyEntities.size();)
			{
				IHasHealth* entity = bloodyEntities[i];
				entity->hurt(dotValue);

				if (entity->isDead())
				{
					bloodyEntities.erase(bloodyEntities.begin() + i); // erase and do NOT increment
				}
				else
				{
					++i; // only increment if not erased
				}
			}
		}
	}
public: 
	POff_DOT_Upgrade() : Perk(INFO, TRIGGERS) {}
};
