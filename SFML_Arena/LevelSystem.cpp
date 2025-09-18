#pragma once

#include "LevelSystem.h" // Own header file
#include "GameInstance.h"
#include "EntityManager.h"
#include "Functions.h"
#include "AllEnemyWaves.h"
#include "SaveGame.h"

void LevelSystem::saveScoreData()
{
	SaveGame::currentData.score = collectedPoints;
}

void LevelSystem::tryLevelUp()
{
	// Check if the player has enough points to level up
	const int needed = getPointsNeeded();
	if (collectedPoints_thisStage >= needed)
	{
		onLevelUp();
	}
}

void LevelSystem::onLevelUp()
{
	collectedPoints_thisStage -= getPointsNeeded(); // Subtract the lastneeded points from the current points of last stage to catch overflow points

	owningInv->getOwner()->resetHealth(); // Reset health after leveling up

	owningInv->addQueuedLevelUp(); // Set the level up flag in the inventory

	const unsigned int nextStage = getStage() + 1;
	setStage(nextStage);  // Increment the stage / level

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
	case 3:
		manager.setSpawnWave<SW_Stage3>();
		break;
	default:
		break;
	}
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

void LevelSystem::addPoints(unsigned int points)
{
	// Increment the score by the specified number of points
	collectedPoints += points;
	collectedPoints_thisStage += points;

	saveScoreData();
	tryLevelUp();
}

void LevelSystem::setPoints(unsigned int points)
{
	// Set the score to the specified number of points
	collectedPoints = points;
	collectedPoints_thisStage = points;

	saveScoreData();
	tryLevelUp();
}

int LevelSystem::getPointsNeeded() const
{
	return getPointsNeeded(stage); // Call the overloaded function with the current stage
}

int LevelSystem::getPointsNeeded(const int forStage) const
{
	// Formula for points needed to level up
	return (5 * forStage + 10);
}

int LevelSystem::getStage() const
{
	return stage;
}

int LevelSystem::getPoints() const
{
	// Return the current score
	return collectedPoints;
}

int LevelSystem::getPoints_thisStage() const
{
	// Return the points collected in the current stage
	return collectedPoints_thisStage;
}