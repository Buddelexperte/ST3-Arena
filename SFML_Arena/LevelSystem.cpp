#pragma once

#include "LevelSystem.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "Functions.h"
#include "AllEnemyWaves.h"
#include "SaveGame.h"

void LevelSystem::onUpdateScore()
{
	SaveGame::currentData.score = collectedPoints;
	tryLevelUp();
}

void LevelSystem::tryLevelUp()
{
	// Check if the player has enough points to level up
	if (collectedPoints >= stage * 100) // Example condition for leveling up
	{
		onLevelUp();
	}
}

void LevelSystem::onLevelUp()
{
	owningInv->setShouldLevelUp(true); // Set the level up flag in the inventory

	stage++;
	std::cout << "Level Up! Current Stage: " << stage << std::endl;

	EntityManager& manager = EntityManager::getInstance();

	switch (stage)
	{
	case 1: case 0:
		manager.setSpawnWave<SW_Stage1>();
		break;
	case 2:
		manager.setSpawnWave<SW_Stage2>();
		break;
	default:
		break;
	}
}

LevelSystem::LevelSystem(Inventory* owningInv)
	: owningInv(owningInv)
{

}

void LevelSystem::addPoint()
{
	// Increment the score by 1
	collectedPoints++;
	onUpdateScore();
}

void LevelSystem::addPoints(unsigned int points)
{
	// Increment the score by the specified number of points
	collectedPoints += points;
	onUpdateScore();
}

void LevelSystem::setPoints(unsigned int points)
{
	// Set the score to the specified number of points
	collectedPoints = points;
	onUpdateScore();
}

int LevelSystem::getPoints() const
{
	// Return the current score
	return collectedPoints;
}
