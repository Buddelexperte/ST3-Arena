#pragma once

class Inventory; // Forward declaration

class LevelSystem
{
private:
	Inventory* owningInv = nullptr; // Pointer to the owning inventory

	unsigned int collectedPoints = 0; // Current score
	unsigned int stage = 1; // Current stage (1 by default) (level of the player essentially)

	void onUpdateScore(); // Update score function
	void tryLevelUp(); // Check if the player leveled up
	void onLevelUp(); // Function to handle level up events

public:
	LevelSystem(Inventory*);
	~LevelSystem() = default;

	// Score management
	void addPoint();
	void addPoints(unsigned int points);
	void setPoints(unsigned int points);
	int getPoints() const;
	int getPointsNeeded() const;

	unsigned int getStage() const;
};

