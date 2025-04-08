#pragma once

#include "LevelSystem.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "Functions.h"
#include "AllEnemyWaves.h"
#include "SaveGame.h"

void LevelSystem::onUpdateScore()
{
	checkLevelUp();
	SaveGame::currentData.score = collectedPoints;
}

void LevelSystem::checkLevelUp()
{
	// Check if the player has enough points to level up
	if (collectedPoints >= stage * 2) // Example condition for leveling up
	{
		onLevelUp();
	}
}

void LevelSystem::onLevelUp()
{
	owningInv->setShouldLevelUp(true); // Set the level up flag in the inventory

	stage++;
	std::cout << "Level Up! Current Stage: " << stage << std::endl;

	std::unique_ptr<EnemySpawnWave> newWave = nullptr;

	switch (stage)
	{
	case 1: case 0:
		newWave = std::make_unique<SW_Stage1>();
		break;
	case 2:
		newWave = std::make_unique<SW_Stage2>();
		break;
	default:
		break;
	}

	EntityManager::getInstance().getEnemySpawner().setSpawnWave(std::move(newWave));
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
