#pragma once

#include "SpawnWave.h" // Own header file

class SW_Stage2 : public EnemySpawnWave
{
private:
	virtual float getEnemyHealth() const override
	{
		static constexpr float ENEMY_HEALTH = 1.45f;
		return ENEMY_HEALTH;
	}

	virtual IMovable::RenderInfo makeSpawnRenderInfo() override
	{
		IMovable::RenderInfo renderInfo = EnemySpawnWave::makeSpawnRenderInfo();
		renderInfo.size *= 1.2f;
		return renderInfo;
	}

public:
	SW_Stage2()
		: EnemySpawnWave(2.0f, 3, 0) // 2s per interval, 4 enemy per interval, inf intervals
	{

	}
	~SW_Stage2() = default;
};