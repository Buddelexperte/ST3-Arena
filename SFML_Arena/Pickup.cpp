#pragma once

#include "Pickup.h" // Own header
#include "GameInstance.h"

Pickup::Pickup()
	: Entity(EntityType::Pickup)
{

}

void Pickup::onCollision(IHasCollision* other)
{
	other->collideWithPickup(*this);
}

void Pickup::collideWithPlayer(Player& player)
{
	onPickup();

	PerkTriggerInfo triggerInfo(PerkTrigger::OnItemPickup, getPosition(), this);
	gameInstance().getInventory().triggerPerks(triggerInfo);

	kill_self();
}
