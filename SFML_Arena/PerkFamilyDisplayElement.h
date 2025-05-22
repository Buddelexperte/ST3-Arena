#pragma once

#include "WidgetElements.h"
#include "PerkInfo.h"

class PerkFamily_Element : public WidgetElement
{
private:
	PerkFamily displayedFamily = PerkFamily::None;

	// Background
	Button B_bg;
	// Perk Family Name
	Text T_name;
	// Perk Family Description
	Text T_desc;
	// Perk Family Icon
	Border I_Icon;

	void setFamilyTexture(const PerkFamily& pf);
	void setFamilyColor(const PerkFamily& pf);

public:
	PerkFamily_Element(InputWidget* parent);
	void construct() override;
	void construct(const sf::Vector2f& startPos, const PerkFamily& pf);

	void tick(const float& deltaTime) override;

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

		B_bg.addPosition(delta, bTickBased);
		T_name.addPosition(delta, bTickBased);
		T_desc.addPosition(delta, bTickBased);
		I_Icon.addPosition(delta, bTickBased);
	}

	bool isMouseOver(const bool& = false) override;

	PerkFamily getDisplayedFamily() const;

	std::function<void()> onSelect = nullptr;
};