#pragma once

#include "SpawnWave.h" // Own header file

class SW_Stage1 : public EnemySpawnWave
{
public:
	SW_Stage1()
		: EnemySpawnWave(1.0f, 1, 0) // 1s per interval, 1 enemy per interval, inf intervals
	{

	}

	~SW_Stage1() = default;
};