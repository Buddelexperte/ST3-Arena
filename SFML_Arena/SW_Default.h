#pragma once

#include "SpawnWave.h" // Own header file

class SW_Default : public EnemySpawnWave
{
public:
	SW_Default()
		: EnemySpawnWave(SMALLEST_PRECISION, 1, 0) // 1 enemy per interval, inf intervals
	{

	}
	~SW_Default() = default;
};