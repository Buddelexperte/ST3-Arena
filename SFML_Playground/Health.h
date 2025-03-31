#pragma once

#include "ValueBar.h"

class IHasHealth
{
private:
	ValueBar healthBar;
	ValueBar& getHealthBar()
	{
		return healthBar;
	}

	virtual void tick_health(const float&) = 0;

public:
	virtual void hurt(const float& delta)
	{
		getHealthBar().addValue(-delta);
	}

	virtual void heal(const float& delta)
	{
		getHealthBar().addValue(delta);
	}

	virtual void setHealth(const float& newVal)
	{
		return getHealthBar().setValue(newVal);
	}

	virtual void setMaxHealth(const float& newMax)
	{
		return getHealthBar().setMaxValue(newMax);
	}

	virtual float getMaxHealth()
	{
		return getHealthBar().getMaxValue();
	}

	virtual float getHealth()
	{
		return getHealthBar().getValue();
	}

	bool isDead()
	{
		return (getHealth() < SMALLEST_PRECISION);
	}

	void resetHealth(const float& newMax)
	{
		getHealthBar().setMaxValue(newMax);
		getHealthBar().reset();
	}

	void resetHealth()
	{
		getHealthBar().reset();
	}
};