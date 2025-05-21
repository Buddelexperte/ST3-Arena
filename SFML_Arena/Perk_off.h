#pragma once

#include "ItemBaseClasses.h"
#include "GameInstance.h"
#include "EntityManager.h"

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

	// TODO: Make effect component class for Entities instead of pointer handling
	std::unordered_map<size_t, IHasHealth*> bloodyEntities;

	void onEnemyGotHit(PerkTriggerInfo& triggerInfo) override
	{
		Entity* actor = triggerInfo.actor;
		if (actor)
		{
			size_t id = actor->getID();
			IHasHealth* healthEntity = dynamic_cast<IHasHealth*>(actor);
			if (healthEntity)
			{
				bloodyEntities[id] = healthEntity;
			}
		}
	}

	void onInterval(const float& deltaTime) override
	{
		dotTimer.addValue(-deltaTime);

		if (dotTimer.isEmpty())
		{
			dotTimer.fill_to_max();

			for (auto it = bloodyEntities.begin(); it != bloodyEntities.end(); )
			{
				size_t id = it->first;
				IHasHealth* entity = it->second;
				bool keyStillValid = EntityManager::getInstance().entityExists(id);

				if (!keyStillValid)
					entity = nullptr;

				if (!entity) {
					it = bloodyEntities.erase(it); // invalid or null entity
					continue;
				}

				entity->hurt(dotValue);

				if (entity->isDead()) {
					it = bloodyEntities.erase(it); // remove dead ones
				}
				else {
					++it; // advance normally
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

	// TODO: Make effect component class for Entities instead of pointer handling
	std::unordered_map<size_t, IHasHealth*> bloodyEntities;

	void onEnemyGotHit(PerkTriggerInfo& triggerInfo) override
	{
		Entity* actor = triggerInfo.actor;
		if (actor)
		{
			size_t id = actor->getID();
			IHasHealth* healthEntity = dynamic_cast<IHasHealth*>(actor);
			if (healthEntity)
			{
				bloodyEntities[id] = healthEntity;
			}
		}
	}

	void onInterval(const float& deltaTime) override
	{
		dotTimer.addValue(-deltaTime);

		if (dotTimer.isEmpty())
		{
			dotTimer.fill_to_max();

			for (auto it = bloodyEntities.begin(); it != bloodyEntities.end(); )
			{
				size_t id = it->first;
				IHasHealth* entity = it->second;
				bool keyStillValid = EntityManager::getInstance().entityExists(id);

				if (!keyStillValid)
					entity = nullptr;

				if (!entity) {
					it = bloodyEntities.erase(it); // invalid or null entity
					continue;
				}

				entity->hurt(dotValue);

				if (entity->isDead()) {
					it = bloodyEntities.erase(it); // remove dead ones
				}
				else {
					++it; // advance normally
				}
			}


		}
	}

public: 
	POff_DOT_Upgrade() : Perk(INFO, TRIGGERS) {}
};
