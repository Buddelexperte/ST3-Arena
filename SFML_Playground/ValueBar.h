#pragma once

#include "Functions.h"

class ValueBar
{
private:
	float currentValue;
	float maxValue;
public:
	ValueBar()
		: maxValue(1.0f), currentValue(1.0f)
	{}

	ValueBar(const float& maxVal)
		: maxValue(maxVal), currentValue(maxVal)
	{}

	ValueBar(const float& maxVal, const float& currentValue)
		: maxValue(maxVal), currentValue(currentValue)
	{}

	void setValue(const float& newVal)
	{
		currentValue = newVal;
		zeroPrecision(currentValue);
	}

	void setMaxValue(const float& newMax)
	{
		maxValue = newMax;
	}

	void addValue(const float& delta)
	{
		currentValue += delta;
		zeroPrecision(currentValue);
	}

	void reset(const float& deficit)
	{
		currentValue = maxValue - deficit;
	}

	float getValue() const
	{
		return currentValue;
	}

	float getMaxValue() const
	{
		return maxValue;
	} 

	bool isMax() const
	{
		return currentValue >= maxValue;
	}

	bool isNotEmpty() const
	{
		return currentValue > 0.0f;
	}

	bool isEmpty() const
	{
		return currentValue <= 0.0f;
	}
};