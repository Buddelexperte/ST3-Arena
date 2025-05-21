#pragma once

#include "WidgetElements.h"
#include "PerkInfo.h"

class PerkFamily_Element : public WidgetElement
{
private:
	PerkFamily displayedFamily = PerkFamily::None;

	sf::Texture familyIcon;

	// Background
	Border border;
	// Perk Family Name
	Button T_name;
	// Perk Family Description
	Button T_desc;
	// Perk Family Icon
	Button I_Icon;

	void setFamilyTexture(const PerkFamily& pf);

public:
	PerkFamily_Element(InputWidget* parent);
	void construct() override
	{

	}
	void construct(const sf::Vector2f& startPos, const PerkFamily& pf)
	{
		displayedFamily = pf;

		setFamilyTexture(pf);
		setPosition(startPos);

		PerkFamilyInfo fInfo = getPerkFInfo(displayedFamily);
		T_name.setText(fInfo.name);
		T_desc.setText(fInfo.description);
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

	bool isMouseOver(const bool& = false) override;

	PerkFamily getDisplayedFamily() const;

	std::function<void()> onSelect = nullptr;
};