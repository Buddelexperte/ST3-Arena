#pragma once

#include "WidgetBase.h"

class PerkFamily_Display : public WidgetElement
{
private:
	// Background
	Button border;
	// Perk Family Name
	Button T_name;
	// Perk Family Description
	Button T_desc;
	// Perk Family Icon
	Button I_Icon;

public:
	PerkFamily_Display(InputWidget* parent);

	void construct(const sf::Vector2f& startPos)
	{
		addPosition(startPos);
	}

	void tick_pos(const sf::Vector2f& withPos)
	{
		WidgetElement::tick_pos(withPos);

		border.tick_pos(withPos);
		T_name.tick_pos(withPos);
		I_Icon.tick_pos(withPos);
		T_desc.tick_pos(withPos);
	}

	void setPosition(const sf::Vector2f& pos) override
	{
		const bool bVelocityBased = false;
		sf::Vector2f posDiff = pos - getPosition();

		WidgetElement::setPosition(pos);

		addPosition(posDiff, bVelocityBased);
	}

	void addPosition(const sf::Vector2f& delta, const bool& = true) override
	{
		WidgetElement::addPosition(delta);

		border.addPos(delta);
		T_name.addPos(delta);
		T_desc.addPos(delta);
		I_Icon.addPos(delta);
	}
};