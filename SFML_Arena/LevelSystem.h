#pragma once

class Inventory; // Forward declaration

class LevelSystem
{
private:
	Inventory* owningInv = nullptr; // Pointer to the owning inventory

	// CONFIGS
	static constexpr int START_STAGE = 0; // Starting stage
	static constexpr int STARTING_POINTS = 0; // Points needed to level up

	int collectedPoints = STARTING_POINTS; // Current score
	int collectedPoints_thisStage = STARTING_POINTS; // Points collected in the current stage
	int stage = START_STAGE; // Current stage (level of the player essentially)

	void saveScoreData(); // Update score function
	void tryLevelUp(); // Check if the player leveled up
	void onLevelUp(); // Function to handle level up events

	void setStage(const unsigned int newStage);

public:
	LevelSystem(Inventory*);
	~LevelSystem() = default;

	void reset();

	// Score management
	void addPoints(unsigned int points);
	void setPoints(unsigned int points);
	int getPoints() const;
	int getPoints_thisStage() const;
	int getPointsNeeded() const;

	// For specific stages
	int getPointsNeeded(const int fromStage) const; 

	int getStage() const;
};