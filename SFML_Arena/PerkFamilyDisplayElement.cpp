#pragma once

#include "PerkFamilyDisplayElement.h" // Own header file

void PerkFamily_Element::setFamilyTexture(const PerkFamily& pf)
{
	static const std::unordered_map<PerkFamily, ETexture> familyPaths = {
		{PerkFamily::Offensive, ETexture::PERK_OFF},
		{PerkFamily::Defensive, ETexture::PERK_DEF},
		{PerkFamily::Utility,   ETexture::PERK_UTIL},
		{PerkFamily::Support,   ETexture::PERK_SUP}
	};

	const auto it = familyPaths.find(pf);
	if (it == familyPaths.end())
	{
		std::cerr << "Unknown PerkFamily enum: " << static_cast<int>(pf) << std::endl;
		return;
	}

	TextureManager& tm = TextureManager::getInstance();
	const sf::Texture* tex_ptr = tm.getTexturePtr(it->second);
	if (tex_ptr != nullptr)
	{
		I_Icon.setTexture(tex_ptr);
	}
}

void PerkFamily_Element::setFamilyColor(const PerkFamily& pf)
{
	static const sf::Color colorDiff = sf::Color(20, 20, 20, 0);

	static const std::unordered_map<PerkFamily, sf::Color> familyPaths = {
		{PerkFamily::Offensive, sf::Color::Red - colorDiff		},
		{PerkFamily::Defensive, sf::Color::Blue + sf::Color(100, 100, 0)},
		{PerkFamily::Utility,   sf::Color::White - colorDiff	},
		{PerkFamily::Support,   sf::Color::Green - colorDiff	}
	};

	const auto it = familyPaths.find(pf);
	if (it == familyPaths.end())
	{
		std::cerr << "Unknown PerkFamily enum: " << static_cast<int>(pf) << std::endl;
		return;
	}

	B_bg.setColor(it->second);
}

PerkFamily_Element::PerkFamily_Element(InputWidget* parent)
	: WidgetElement(parent),
	B_bg(parent), T_name(parent), I_Icon(parent), T_desc(parent)
{
	const std::vector<RawText> TEXT_CONTRS = {
		{(sf::Vector2f{ 0.0f, -330.0f } *viewSizeNorm),	sf::Color::Black,	16,	"Family Name",							EAlignment::CENTER_TOP},
		{(sf::Vector2f{ 0.0f, 0.0f } *viewSizeNorm),	sf::Color::Black,	16,	"Family Description\n- Stats\n- Lore",	EAlignment::CENTER_TOP}
	};

	const std::vector<RawBorder> ICON_CONSTR = {
		{(sf::Vector2f{ 0.0f, -230.0f } * viewSizeNorm),	sf::Vector2f{ 160.0f, 160.0f } * viewSizeNorm, sf::Color::Red, EAlignment::CENTER_TOP}
	};

	const std::vector<RawButton> BUTTON_CONSTR = {
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),		sf::Vector2f{ 300.0f, 700.0f } * viewSizeNorm, backgroundInterfaceColor, EAlignment::CENTER }
	};
	
	TextureManager& tm = TextureManager::getInstance();

	B_bg.construct(BUTTON_CONSTR[0]);
	B_bg.setTexture(tm.getTexturePtr(ETexture::PERK_FAMILY_BORDER));
	T_name.construct(TEXT_CONTRS[0]);
	T_desc.construct(TEXT_CONTRS[1]);
	I_Icon.construct(ICON_CONSTR[0]);


	shapes = { &B_bg, &T_name, &I_Icon, &T_desc };
}

void PerkFamily_Element::construct()
{
	B_bg.construct();
}

void PerkFamily_Element::construct(const sf::Vector2f& startPos, const PerkFamily& pf)
{
	if (displayedFamily == pf)
		construct();

	displayedFamily = pf;

	setFamilyTexture(pf);
	setFamilyColor(pf);
	setPosition(startPos);

	PerkFamilyInfo fInfo = getPerkFInfo(displayedFamily);
	T_name.setText(fInfo.name);
	T_desc.setText(fInfo.description);
}

void PerkFamily_Element::tick(const float& deltaTime)
{
	WidgetElement::tick(deltaTime);

	B_bg.tick(deltaTime);
}

bool PerkFamily_Element::isMouseOver(const bool& bCheckForClick)
{
	const bool isMouseOver = B_bg.isMouseOver(bCheckForClick);

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
