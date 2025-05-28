#pragma once

class Inventory; // Forward declaration

class LevelSystem
{
private:
	Inventory* owningInv = nullptr; // Pointer to the owning inventory

	// CONFIGS
	static constexpr unsigned int START_STAGE = 0; // Starting stage
	static constexpr unsigned int STARTING_POINTS = 0; // Points needed to level up

	unsigned int collectedPoints = STARTING_POINTS; // Current score
	unsigned int stage = START_STAGE; // Current stage (level of the player essentially)

	void onUpdateScore(); // Update score function
	void tryLevelUp(); // Check if the player leveled up
	void onLevelUp(); // Function to handle level up events

	void setStage(const unsigned int newStage);

	unsigned int calculatePointsNeeded(const unsigned int stage) const; // Calculate points needed for a specific stage

public:
	LevelSystem(Inventory*);
	~LevelSystem() = default;

	void reset();

	// Score management
	void addPoint();
	void addPoints(unsigned int points);
	void setPoints(unsigned int points);
	int getPoints() const;
	unsigned int getPointsNeeded() const;
	unsigned int getLastPointsNeeded() const;
	unsigned int getPointsNeeded(const unsigned int) const; // For specific stages
	unsigned int getLastPointsNeeded(const unsigned int) const; // For specific stages

	unsigned int getStage() const;
};

