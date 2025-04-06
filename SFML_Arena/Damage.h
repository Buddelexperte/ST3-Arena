#pragma once

class IHasDamage
{
private:
	float damage = 0.0f;

public:
	void setDamage(const float& newDamage)
	{
		damage = newDamage;
	}

	float getDamage() const
	{
		return damage;
	}
};