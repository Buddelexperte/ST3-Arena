#pragma once

#include "Particle.h"

class P_Sparkle : public Particle
{
private:
	sf::Vector2f startSize;
	float rotationSpeed = 0.0f;

	void tick_move(const float&) override;

public:
	P_Sparkle();
	~P_Sparkle() = default;

	void spawn(const SpawnInformation&) override;
	void releaseToPool() override;
};