#pragma once

#include "WidgetElements.h"

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
		setPosition(startPos);
	}

	void setPosition(const sf::Vector2f& pos) override
	{
		constexpr bool bTickBased = false;
		sf::Vector2f posDiff = pos - getPosition();

		WidgetElement::setPosition(pos);

		addPosition(posDiff, bTickBased);
	}

	void addPosition(const sf::Vector2f& delta, const bool& bTickBased = true) override
	{
		WidgetElement::addPosition(delta, bTickBased);

		border.addPosition(delta, bTickBased);
		T_name.addPosition(delta, bTickBased);
		T_desc.addPosition(delta, bTickBased);
		I_Icon.addPosition(delta, bTickBased);
	}
};