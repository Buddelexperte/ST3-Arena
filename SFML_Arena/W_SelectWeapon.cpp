#pragma once

#include "W_SelectWeapon.h" // Own header file
#include "GameInstance.h"

W_SelectWeapon::W_SelectWeapon(InputWidget* parent) 
	: InputWidget(parent),
	WEAPON_SPACING_X(viewHalfSize.x / NUM_WEAPONS),
	T_Title(this), 
	B_Pistol(this), B_Rifle(this), B_Rifle_Burst(this), B_Shotgun(this), 
	T_Pistol(this), T_Rifle(this), T_Rifle_Burst(this), T_Shotgun(this),
	B_Return(this)
{
	const std::vector<RawText> TITLE_CONSTR = {
		{sf::Vector2f(0.0f						, -300.0f)					* unitNorm, sf::Color::White,		100,	"SELECT WEAPON"},
		{sf::Vector2f(-1.5f * WEAPON_SPACING_X	, 25.0f + padding * 2.0f)	* unitNorm, sf::Color::White,		24,		"DEFAULT WEAPON"},
		{sf::Vector2f(-0.5f * WEAPON_SPACING_X	, 25.0f + padding * 2.0f)	* unitNorm, sf::Color::White,		24,		""},
		{sf::Vector2f(0.5f * WEAPON_SPACING_X	, 25.0f + padding * 2.0f)	* unitNorm, sf::Color::White,		24,		""},
		{sf::Vector2f(1.5f * WEAPON_SPACING_X	, 25.0f + padding * 2.0f)	* unitNorm, sf::Color::White,		24,		""},
	};

	const std::vector<RawButton> INVENTORY_MENU_CONSTR = {
		{sf::Vector2f(-1.5f * WEAPON_SPACING_X, -75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	WEAPON_SELECT_COLOR,	24,		"PISTOL",		sf::Color::White},
		{sf::Vector2f(-0.5f * WEAPON_SPACING_X,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	WEAPON_SELECT_COLOR,	24,		"BURST RIFLE",	sf::Color::White},
		{sf::Vector2f(0.5f * WEAPON_SPACING_X,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	WEAPON_SELECT_COLOR,	24,		"RIFLE",		sf::Color::White},
		{sf::Vector2f(1.5f * WEAPON_SPACING_X, -75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	WEAPON_SELECT_COLOR,	24,		"SHOTGUN",		sf::Color::White},
		{sf::Vector2f(0.0f,						150.0f) * unitNorm,		buttonSize,									sf::Color::White,		24,		"RETURN",		sf::Color::Black}
	};

	static constexpr bool lockedWeapon = true;

	T_Title.construct(TITLE_CONSTR[0]);

	T_Pistol.construct(TITLE_CONSTR[1]);
	T_Rifle_Burst.construct(TITLE_CONSTR[2]);
	T_Rifle.construct(TITLE_CONSTR[3]);
	T_Shotgun.construct(TITLE_CONSTR[4]);

	B_Pistol.construct(INVENTORY_MENU_CONSTR[0]);
	B_Rifle_Burst.construct(INVENTORY_MENU_CONSTR[1], lockedWeapon);
	B_Rifle.construct(INVENTORY_MENU_CONSTR[2], lockedWeapon);
	B_Shotgun.construct(INVENTORY_MENU_CONSTR[3], lockedWeapon);
	B_Return.construct(INVENTORY_MENU_CONSTR[4]);

	weaponButtons = {
		&B_Pistol, &B_Rifle, &B_Rifle_Burst, &B_Shotgun,
	};

	delegateEvents();

	shapes = { 
		&T_Title, 
		&B_Pistol, &B_Rifle, &B_Rifle_Burst, &B_Shotgun,  
		&T_Pistol, &T_Rifle, &T_Rifle_Burst, &T_Shotgun,
		&B_Return
	};
}

void W_SelectWeapon::delegateEvents()
{
	B_Return.onClick = [this]()
	{
		onKeyEscape();
	};

	// Every weapon select button should do this onClick
	for (Button* button : weaponButtons)
	{
		if (button == nullptr)
			continue;

		button->onClick = [this, button]() // Not a &button reference as this is a pointer and can be copied
		{
			selectedWeaponName = button->getText();
			tryStartGame();
		};
	}
}

void W_SelectWeapon::construct()
{
	InputWidget::construct();

	gameInstance().addToDefaultWindowName(" - Selecting Weapon...");
	setWidgetIndex(0);

	checkForWeaponUnlock();

	T_Title.construct();

	B_Pistol.construct();
	B_Rifle.construct();
	B_Rifle_Burst.construct();
	B_Shotgun.construct();

	T_Pistol.construct();
	T_Rifle.construct();
	T_Rifle_Burst.construct();
	T_Shotgun.construct();

	B_Return.construct();
}

void W_SelectWeapon::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);

	B_Pistol.tick(deltaTime);
	B_Rifle.tick(deltaTime);
	B_Rifle_Burst.tick(deltaTime);
	B_Shotgun.tick(deltaTime);

	T_Pistol.tick(deltaTime);
	T_Rifle.tick(deltaTime);
	T_Rifle_Burst.tick(deltaTime);
	T_Shotgun.tick(deltaTime);

	B_Return.tick(deltaTime);
}

bool W_SelectWeapon::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	if (B_Return.isMouseOver(checkForClick))
		return true;

	selectedWeaponName = "NoWeapon";

	for (Button* button : weaponButtons)
	{
		if (button == nullptr)
			continue;

		if (button->isMouseOver(checkForClick))
			return true;
	}

	return false;
}

bool W_SelectWeapon::onKeyEscape()
{
	if (isChildActive())
		return getActiveChild()->onKeyEscape();

	if (parent != nullptr)
	{
		// Overridden this to only go back ONE widget instead of whole MainMenu
		parent->setWidgetIndex(parent->getWidgetIndex() - 1)->construct();
		return true;
	}

	return false;
}

void W_SelectWeapon::tryStartGame()
{
	gameInstance().getPlayer()->getInventory().setStartWeapon(selectedWeaponName);
	gameInstance().resetWindowName();
	parent->playAnim(CLOSE_ANIM);
}

void W_SelectWeapon::checkForWeaponUnlock()
{
	static const sf::Color LOCKED_COLOR = grayedOutButtonColor;
	static const sf::Color UNLOCKED_COLOR = sf::Color::White;
	static const std::string UNLOCKED_STR = "UNLOCKED";

	const int highscore = SaveGame::getCurrHighscore().highscore;

	const std::vector<WeaponUnlockInfo> weapons = {
		{ &B_Rifle_Burst, &T_Rifle_Burst,"LOCKED: 5000 HIGHSCORE",	5000 },
		{ &B_Rifle,       &T_Rifle,		 "LOCKED: 10000 HIGHSCORE",	10000 },
		{ &B_Shotgun,     &T_Shotgun,	 "LOCKED: 50000 HIGHSCORE",	50000 }
	};

	for (const auto& weapon : weapons)
	{
		bool unlocked = highscore >= weapon.requiredScore;

		weapon.button->setEnabled(unlocked);
		weapon.label->setText(unlocked ? UNLOCKED_STR : weapon.lockedText);
		weapon.label->setColor(unlocked ? UNLOCKED_COLOR : LOCKED_COLOR);

		weapon.button->playAnim(unlocked ? EAnimation::ON_UNHOVER : EAnimation::ON_HOVER);
	}

	B_Pistol.playAnim(EAnimation::ON_UNHOVER);
}