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
	static const std::unordered_map<PerkFamily, sf::Color> familyPaths = {
		{PerkFamily::Offensive, sf::Color::Red},
		{PerkFamily::Defensive, sf::Color::Blue},
		{PerkFamily::Utility,   sf::Color::White},
		{PerkFamily::Support,   sf::Color::Green}
	};

	const auto it = familyPaths.find(pf);
	if (it == familyPaths.end())
	{
		std::cerr << "Unknown PerkFamily enum: " << static_cast<int>(pf) << std::endl;
		return;
	}

	const sf::Color whiteDiff = (sf::Color::White - it->second);

	sf::Color bgColor = it->second - (it->second * 0.4f) + (whiteDiff * 0.2f);
	bgColor.a = 200;
	B_bg.setColor(bgColor);

	sf::Color nameBoxColor = (whiteDiff * 0.65f) + it->second;
	nameBoxColor.a = 150;
	B_name.setColor(nameBoxColor);
}

PerkFamily_Element::PerkFamily_Element(InputWidget* parent)
	: WidgetElement(parent),
	B_bg(parent), B_name(parent), I_Icon(parent), T_desc(parent)
{
	const std::vector<RawText> TEXT_CONTRS = {
		{(sf::Vector2f{ 0.0f, 200.0f } * viewSizeNorm),	sf::Color::Black,	16,	"Family Description\n- Stats\n- Lore",	EAlignment::CENTER_TOP}
	};

	const std::vector<RawBorder> ICON_CONSTR = {
		{(sf::Vector2f{ 0.0f, -230.0f } * viewSizeNorm),	sf::Vector2f{ 160.0f, 160.0f } * unitNorm, sf::Color::Red, EAlignment::CENTER_TOP}
	};

	const std::vector<RawButton> BUTTON_CONSTR = {
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),	sf::Vector2f{ 300.0f, 700.0f } * unitNorm,	backgroundInterfaceColor, EAlignment::CENTER },
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),	buttonSize * 0.8f,								backgroundInterfaceColor,	24,	"Family Name", normalTextColor, EAlignment::CENTER_TOP}
	};
	
	TextureManager& tm = TextureManager::getInstance();

	B_bg.construct(BUTTON_CONSTR[0]);
	B_bg.setTexture(tm.getTexturePtr(ETexture::PERK_FAMILY_BORDER));
	B_name.construct(BUTTON_CONSTR[1]);
	B_name.setTexture(buttonTexture);
	T_desc.construct(TEXT_CONTRS[0]);
	I_Icon.construct(ICON_CONSTR[0]);


	shapes = { &B_bg, &B_name, &I_Icon, &T_desc };
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
	B_name.setText(fInfo.name);
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
