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
	if (collectedPoints >= getPointsNeeded()) // Example condition for leveling up
	{
		onLevelUp();
	}
}

void LevelSystem::onLevelUp()
{
	owningInv->setShouldLevelUp(true); // Set the level up flag in the inventory

	setStage(stage + 1);  // Increment the stage / level

	std::cout << "Level Up! Current Stage: " << stage << std::endl;
}

void LevelSystem::setStage(const unsigned int newStage)
{
	stage = newStage;

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

unsigned int LevelSystem::calculatePointsNeeded(const unsigned int stage) const
{
	return (10 + stage * 100);
}

LevelSystem::LevelSystem(Inventory* owningInv)
	: owningInv(owningInv)
{

}

void LevelSystem::reset()
{
	setPoints(STARTING_POINTS);
	setStage(START_STAGE);
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

unsigned int LevelSystem::getPointsNeeded() const
{
	return getPointsNeeded(stage); // Call the overloaded function with the current stage
}

unsigned int LevelSystem::getLastPointsNeeded() const
{
	return getLastPointsNeeded(stage); // Call the overloaded function with the current stage
}

unsigned int LevelSystem::getPointsNeeded(const unsigned int forStage) const
{
	return calculatePointsNeeded(forStage);
}

unsigned int LevelSystem::getLastPointsNeeded(const unsigned int fromStage) const
{
	if (fromStage == 0)
		return 0;

	return getPointsNeeded(fromStage - 1); // Call the overloaded function with the previous stage
}

unsigned int LevelSystem::getStage() const
{
	return stage;
}
