#pragma once

#include "PerkFamilyDisplay.h" // Own header file

PerkFamily_Display::PerkFamily_Display(InputWidget* parent)
	: WidgetElement(parent)
{
	const std::vector<RawButton> PERK_FAMILY_CONSTR = {
		// Background (border)
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),		sf::Vector2f{ 300.0f, 700.0f } * viewSizeNorm,		sf::Color(140, 140, 140, 255),	0,	"",										sf::Color::Black,	EAlignment::CENTER,		EAlignment::CENTER_TOP},
		// Name
		{(sf::Vector2f{ 0.0f, -330.0f } * viewSizeNorm),	sf::Vector2f{ 150.0f, 40.0f } * viewSizeNorm,		sf::Color(100, 100, 100, 255),	16,	"Family Name",							sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::CENTER},
		// Icon
		{(sf::Vector2f{ 0.0f, -270.0f } * viewSizeNorm),	sf::Vector2f{ 220.0f, 220.0f } * viewSizeNorm,		sf::Color::Red,					0,	"",										sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::CENTER},
		// Description
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),		sf::Vector2f{ 180.0f, 330.0f } * viewSizeNorm,		sf::Color::Transparent,			16,	"Family Description\n- Stats\n- Lore",	sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::LEFT_TOP}
	
	};
	
	border.construct(PERK_FAMILY_CONSTR[0]);
	T_name.construct(PERK_FAMILY_CONSTR[1]);
	I_Icon.construct(PERK_FAMILY_CONSTR[2]);
	T_desc.construct(PERK_FAMILY_CONSTR[3]);

	shapes = { &border, &T_name, &I_Icon, &T_desc };
}