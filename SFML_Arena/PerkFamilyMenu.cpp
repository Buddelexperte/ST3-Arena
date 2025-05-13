#pragma once

#include "PerkFamilyMenu.h" // Own header file

PerkFamilyMenu::PerkFamilyMenu(InputWidget* parent)
	: InputWidget(parent),
	B_Skip(this),
	pf_display_1(this), pf_display_2(this), pf_display_3(this), pf_display_4(this)
{
	const RawButton CONSTR_SKIP = { (sf::Vector2f{ 0, 480.0f } *viewSizeNorm), buttonSize, sf::Color(255, 255, 255, 255),	24,	" S K I P",	sf::Color::Black, EAlignment::CENTER_BOTTOM, EAlignment::CENTER };

	B_Skip.construct(CONSTR_SKIP);

	const std::vector<sf::Vector2f> FAMILY_POS = {
		(sf::Vector2f{ -690.0f, -50.0f } *viewSizeNorm),
		(sf::Vector2f{ -230.0f, -50.0f } *viewSizeNorm),
		(sf::Vector2f{ 230.0f, -50.0f } *viewSizeNorm),
		(sf::Vector2f{ 690.0f, -50.0f } *viewSizeNorm)
	};

	pf_display_1.construct(FAMILY_POS[0], PerkFamily::Offensive);
	pf_display_2.construct(FAMILY_POS[1], PerkFamily::Defensive);
	pf_display_3.construct(FAMILY_POS[2], PerkFamily::Utility);
	pf_display_4.construct(FAMILY_POS[3], PerkFamily::Support);

	delegateEvents();

	shapes = { &B_Skip, &pf_display_1, &pf_display_2, &pf_display_3, &pf_display_4 };
}

void PerkFamilyMenu::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	B_Skip.tick(deltaTime);
	
	for (auto& familyDisplay : familyDisplays)
		familyDisplay->tick(deltaTime);
}

void PerkFamilyMenu::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);

	B_Skip.construct();
	
	for (auto& familyDisplay : familyDisplays)
		familyDisplay->construct();
}

bool PerkFamilyMenu::isMouseOver(const bool& bCheckForClick)
{
	if (B_Skip.isMouseOver(bCheckForClick))
		return true;

	for (auto& familyDisplay : familyDisplays)
	{
		if (familyDisplay->isMouseOver(bCheckForClick))
			return true;
	}

	// On no button-mouse overlap
	return false;
}

void PerkFamilyMenu::delegateEvents()
{
	B_Skip.onClick = [this]()
	{
		if (parent)
			parent->onKeyEscape(); // Parent is the W_LevelUp widget, which should close instead of this subwidget
	};


	// Getting all family displays accounted
	for (auto& familyDisplay : familyDisplays)
	{
		familyDisplay->onSelect = [this, familyDisplay]()
			{
				if (onSelectFamily)
					onSelectFamily(familyDisplay->getDisplayedFamily());
			};
	}


}
