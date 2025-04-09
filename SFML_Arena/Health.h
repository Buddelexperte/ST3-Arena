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

	virtual void onHealthChanged() {};

public:
	virtual void hurt(const float& delta)
	{
		getHealthBar().addValue(-delta);
		onHealthChanged();
	}

	virtual void heal(const float& delta)
	{
		getHealthBar().addValue(delta);
		onHealthChanged();
	}

	virtual void setHealth(const float& newVal)
	{
		return getHealthBar().setValue(newVal);
		onHealthChanged();
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
		resetHealth();
	}

	void resetHealth()
	{
		getHealthBar().reset();
		onHealthChanged();
	}
};