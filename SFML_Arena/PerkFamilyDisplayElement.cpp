#pragma once

#include "PerkFamilyDisplayElement.h" // Own header file

void PerkFamily_Element::setFamilyTexture(const PerkFamily& pf)
{
	static const std::string DIR = "Content/Textures/Icons/";

	static const std::unordered_map<PerkFamily, std::string> familyPaths = {
		{PerkFamily::Offensive, "sword.png"},
		{PerkFamily::Defensive, "shield.png"},
		{PerkFamily::Utility,   "utility.png"},
		{PerkFamily::Support,   "medkit.png"}
	};

	const auto it = familyPaths.find(pf);
	if (it == familyPaths.end())
	{
		std::cerr << "Unknown PerkFamily enum: " << static_cast<int>(pf) << std::endl;
		return;
	}

	const std::string iconPath = DIR + it->second;

	if (!familyIcon.loadFromFile(iconPath))
	{
		std::cerr << "Failed to load texture: " << iconPath << std::endl;
	}
	else
	{
		static constexpr bool resetTint = true;
		I_Icon.setTexture(familyIcon, resetTint);
	}
}

PerkFamily_Element::PerkFamily_Element(InputWidget* parent)
	: WidgetElement(parent),
	border(parent), T_name(parent), I_Icon(parent), T_desc(parent)
{
	const RawBorder CONSTR_BG = {
		sf::Vector2f{ 0.0f, 0.0f } *viewSizeNorm, sf::Vector2f{ 300.0f, 700.0f } *viewSizeNorm, sf::Color(180, 180, 180, 255), EAlignment::CENTER
	};

	const std::vector<RawButton> PERK_FAMILY_CONSTR = {
		// Name																				 
		{(sf::Vector2f{ 0.0f, -330.0f } * viewSizeNorm),	sf::Vector2f{ 150.0f, 40.0f } * viewSizeNorm,		sf::Color(100, 100, 100, 255),	16,	"Family Name",							sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::CENTER},
		// Icon																				 
		{(sf::Vector2f{ 0.0f, -230.0f } * viewSizeNorm),	sf::Vector2f{ 160.0f, 160.0f } * viewSizeNorm,		sf::Color::Red,					0,	"",										sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::CENTER},
		// Description																		 
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),		sf::Vector2f{ 300.0 - (2 * padding), 350.0f - padding } *viewSizeNorm,		sf::Color::Transparent,			16,	"Family Description\n- Stats\n- Lore",	sf::Color::Black,	EAlignment::CENTER_TOP,	EAlignment::LEFT_TOP}
	
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
