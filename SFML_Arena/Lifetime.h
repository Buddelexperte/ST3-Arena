#pragma once

#include "ValueBar.h"

class IHasLifetime
{
private:
	ValueBar lifetime;

protected:
	IHasLifetime(const float& lifetime) 
		: lifetime(lifetime)
	{

	}

	virtual void timeout() = 0;

	void tick_lifetime(const float& deltaTime)
	{
		lifetime.addValue(-deltaTime);

		if (lifetime.isEmpty())
		{
			timeout();
		}
	};

	void resetLifetime(const float& newLifetime)
	{
		lifetime.setMaxValue(newLifetime);

		lifetime.reset();
	}

	ValueBar& getLifetime()
	{
		return lifetime;
	}
};