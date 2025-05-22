#pragma once

#include "W_LevelMenu.h" // Own header file
#include "GameInstance.h"

W_LevelMenu::W_LevelMenu(InputWidget* parent) 
	: InputWidget(parent),
	T_Title(this), 
	B_Level1(this), B_Level2(this), B_Level3(this),
	T_Level1(this), T_Level2(this), T_Level3(this),
	B_Return(this)
{
	const std::vector<RawText> TITLE_CONSTR = {
		{sf::Vector2f(0.0f, -300.0f) * unitNorm, sf::Color::White, 100, "LEVEL SELECT"},
		{sf::Vector2f(-1.0f * LEVEL_DIST, 25.0f + padding * 2.0f) * unitNorm, sf::Color::White,	24,	"DEFAULT LEVEL"},
		{sf::Vector2f(0.0f * LEVEL_DIST	, 25.0f + padding * 2.0f) * unitNorm, sf::Color::White,	24,	""},
		{sf::Vector2f(1.0f * LEVEL_DIST	, 25.0f + padding * 2.0f) * unitNorm, sf::Color::White,	24,	""}
	};

	const std::vector<RawButton> LEVEL_MENU_CONSTR = {
		{sf::Vector2f(-1.0f * LEVEL_DIST,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	darkerButtonColor,	24,		"LEVEL 1",		normalTextColor},
		{sf::Vector2f(0.0f * LEVEL_DIST,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	darkerButtonColor,	24,		"LEVEL 2",		normalTextColor},
		{sf::Vector2f(1.0f * LEVEL_DIST,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	darkerButtonColor,	24,		"LEVEL 3",		normalTextColor},
		{sf::Vector2f(0.0f,		150.0f)				* unitNorm,		buttonSize,									darkerButtonColor,	24,		"RETURN",		normalTextColor}
	};

	T_Title.construct(TITLE_CONSTR[0]);

	T_Level1.construct(TITLE_CONSTR[1]);
	T_Level2.construct(TITLE_CONSTR[2]);
	T_Level3.construct(TITLE_CONSTR[3]);

	TextureManager& tm = TextureManager::getInstance();

	B_Level1.construct(LEVEL_MENU_CONSTR[0]);
	B_Level1.setTexture(tm.getTexturePtr(ETexture::BUTTON_SQR));
	B_Level2.construct(LEVEL_MENU_CONSTR[1]);
	B_Level2.setTexture(tm.getTexturePtr(ETexture::BUTTON_SQR));
	B_Level3.construct(LEVEL_MENU_CONSTR[2]);
	B_Level3.setTexture(tm.getTexturePtr(ETexture::BUTTON_SQR));
	
	B_Return.construct(LEVEL_MENU_CONSTR[3]);
	B_Return.setTexture(buttonTexture);

	delegateEvents();

	shapes = { 
		&T_Title, 
		&B_Level1, &B_Level2, &B_Level3, 
		&T_Level1, &T_Level2, &T_Level3, 
		&B_Return
	};
}

void W_LevelMenu::delegateEvents()
{
	B_Level1.onClick = [this]()
	{
		selectLevel(1);
	};

	B_Level2.onClick = [this]()
	{
		selectLevel(2);
	};

	B_Level3.onClick = [this]()
	{
		selectLevel(3);
	};

	B_Return.onClick = [this]()
	{
		onKeyEscape();
	};
}


void W_LevelMenu::construct()
{
	InputWidget::construct();

	gameInstance().addToDefaultWindowName(" - Selecting Level...");

	checkForWeaponUnlock();

	T_Title.construct();

	B_Level1.construct();
	B_Level2.construct();
	B_Level3.construct();

	T_Level1.construct();
	T_Level2.construct();
	T_Level3.construct();

	B_Return.construct();
}

void W_LevelMenu::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);

	B_Level1.tick(deltaTime);
	B_Level2.tick(deltaTime);
	B_Level3.tick(deltaTime);
	
	T_Level1.tick(deltaTime);
	T_Level2.tick(deltaTime);
	T_Level3.tick(deltaTime);

	B_Return.tick(deltaTime);
}


bool W_LevelMenu::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	if (B_Level1.isMouseOver(checkForClick) 
		|| B_Level2.isMouseOver(checkForClick) 
		|| B_Level3.isMouseOver(checkForClick))
		return true;

	if (B_Return.isMouseOver(checkForClick))
		return true;

	return false;
}

void W_LevelMenu::selectLevel(const unsigned int level)
{
	gameInstance().setSelectedLevel(level);

	parent->setWidgetIndex(parent->getWidgetIndex() + 1)->construct();
}

void W_LevelMenu::checkForWeaponUnlock()
{
	static const sf::Color LOCKED_COLOR = disabledButtonColor;
	static const sf::Color UNLOCKED_COLOR = sf::Color::White;
	static const std::string UNLOCKED_STR = "UNLOCKED";

	const int highest_secondsSurvived = SaveGame::getCurrHighscore().max_secondsSurvived;

	const std::vector<LevelUnlockInfo> levels = {
		{ &B_Level2, &T_Level2,	"LOCKED: 300 SECONDS",	300 },
		{ &B_Level3, &T_Level3,	"LOCKED: 600 SECONDS",	600 }
	};

	for (const auto& level : levels)
	{
		bool unlocked = highest_secondsSurvived >= level.requiredSeconds;

		level.button->setEnabled(unlocked);
		level.label->setText(unlocked ? UNLOCKED_STR : level.lockedText);
		level.label->setColor(unlocked ? UNLOCKED_COLOR : LOCKED_COLOR);

		level.button->playAnim(unlocked ? EAnimation::ON_UNHOVER : EAnimation::ON_HOVER);
	}
}