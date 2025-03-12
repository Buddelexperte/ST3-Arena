#pragma once

#include "ValueBar.h"

class IHasHealth
{
private:
	virtual ValueBar& getValueBar() = 0;

public:
	virtual void hurt(const float& delta)
	{
		getValueBar().addValue(-delta);
	}

	virtual void heal(const float& delta)
	{
		getValueBar().addValue(delta);
	}

	virtual float getHealth()
	{
		return getValueBar().getValue();
	}

	bool isDead()
	{
		return shouldZero(getHealth());
	}
};