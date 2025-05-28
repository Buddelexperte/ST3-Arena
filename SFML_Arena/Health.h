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
	virtual ~IHasHealth() = default;

	virtual void hurt(const float& delta)
	{
		if (delta < 0.0f) return;

		if (getHealth() - delta < 0.0f)
		{
			setHealth(0.0f);
		}
		else
		{
			getHealthBar().addValue(-delta);
		}

		onHealthChanged();
	}

	virtual void heal(const float& delta)
	{
		if (delta < 0.0f) return;

		if (getHealth() + delta > getMaxHealth())
		{
			setHealth(getMaxHealth());
		}
		else
		{
			getHealthBar().addValue(delta);
		}

		onHealthChanged();
	}

	virtual void setHealth(float newVal)
	{
		if (newVal < 0.0f)
			newVal = 0.0f;

		getHealthBar().setValue(newVal);
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

	virtual void resetHealth()
	{
		getHealthBar().fill_to_max();
		onHealthChanged();
	}
};