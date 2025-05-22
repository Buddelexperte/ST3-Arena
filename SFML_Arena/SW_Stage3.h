#pragma once

#include "SpawnWave.h" // Own header file

class SW_Stage3 : public EnemySpawnWave
{
public:
	SW_Stage3()
		: EnemySpawnWave(0.5f, 3, 0) // 0.5s per interval, 3 enemy per interval, inf intervals
	{

	}
	~SW_Stage3() = default;
};