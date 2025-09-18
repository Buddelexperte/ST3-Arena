#pragma once

#include "SpawnWave.h" // Own header file

class SW_Stage3 : public EnemySpawnWave
{
public:
	SW_Stage3()
		: EnemySpawnWave(3.0f, 4, 0) // 0.5s per interval, 4 enemy per interval, inf intervals
	{

	}
	~SW_Stage3() = default;
};