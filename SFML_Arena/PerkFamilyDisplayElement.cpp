#pragma once

#include "PerkFamilyDisplayElement.h" // Own header file

PerkFamily_Element::PerkFamily_Element(InputWidget* parent)
	: WidgetElement(parent),
	border(parent), T_name(parent), I_Icon(parent), T_desc(parent)
{
	const RawBorder CONSTR_BG = {
		sf::Vector2f{ 0.0f, 0.0f } *viewSizeNorm,
		sf::Vector2f{ 300.0f, 700.0f } *viewSizeNorm,
		sf::Color(180, 180, 180, 255),
		EAlignment::CENTER
	};

	const std::vector<RawButton> PERK_FAMILY_CONSTR = {
		// Name																				 
		{(sf::Vector2f{ 0.0f, -330.0f } * viewSizeNorm),	sf::Vector2f{ 150.0f, 40.0f } * viewSizeNorm,		sf::Color(100, 100, 100, 255),	16,	"Family Name",							sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::CENTER},
		// Icon																				 
		{(sf::Vector2f{ 0.0f, -270.0f } * viewSizeNorm),	sf::Vector2f{ 220.0f, 220.0f } * viewSizeNorm,		sf::Color::Red,					0,	"",										sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::CENTER},
		// Description																		 
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),		sf::Vector2f{ 180.0f, 330.0f } * viewSizeNorm,		sf::Color::Transparent,			16,	"Family Description\n- Stats\n- Lore",	sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::LEFT_TOP}
	
	};
	
	border.construct(CONSTR_BG);
	T_name.construct(PERK_FAMILY_CONSTR[0]);
	I_Icon.construct(PERK_FAMILY_CONSTR[1]);
	T_desc.construct(PERK_FAMILY_CONSTR[2]);

	shapes = { &border, &T_name, &I_Icon, &T_desc };
}

bool PerkFamily_Element::isMouseOver(const bool& bCheckForClick)
{
	const bool isMouseOver = border.isMouseOver(bCheckForClick);

	if (!isMouseOver)
		return false;

	if (!bCheckForClick)
		return true;

	if (onSelect != nullptr)
		onSelect();

	return true;
}

PerkFamily PerkFamily_Element::getDisplayedFamily() const
{
	return displayedFamily;
}
