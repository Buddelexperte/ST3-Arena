#pragma once

#include "W_SelectWeapon.h" // Own header file
#include "GameInstance.h"

W_SelectWeapon::W_SelectWeapon(InputWidget* parent) 
	: InputWidget(parent),
	WEAPON_SPACING_X(viewHalfSize.x / NUM_WEAPONS),
	T_Title(this), 
	B_item1(this), B_item2(this), B_item3(this), B_item4(this), 
	B_Return(this)
{
	const RawText TITLE_CONSTR = {
		sf::Vector2f(0.0f, -300.0f) * unitNorm, sf::Color::White, 100, "SELECT WEAPON"
	};

	const std::vector<RawButton> INVENTORY_MENU_CONSTR = {
		{sf::Vector2f(-1.5f * WEAPON_SPACING_X, -75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"PISTOL",		sf::Color::White},
		{sf::Vector2f(-0.5f * WEAPON_SPACING_X, -75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"SHOTGUN",		sf::Color::White},
		{sf::Vector2f(0.5f * WEAPON_SPACING_X,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"RIFLE",		sf::Color::White},
		{sf::Vector2f(1.5f * WEAPON_SPACING_X,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"BURST RIFLE",	sf::Color::White},
		{sf::Vector2f(0.0f,						150.0f) * unitNorm,		buttonSize,									sf::Color::White,			24,		"RETURN",		sf::Color::Black}
	};

	T_Title.construct(TITLE_CONSTR);
	B_item1.construct(INVENTORY_MENU_CONSTR[0]);
	B_item2.construct(INVENTORY_MENU_CONSTR[1]);
	B_item3.construct(INVENTORY_MENU_CONSTR[2]);
	B_item4.construct(INVENTORY_MENU_CONSTR[3]);
	B_Return.construct(INVENTORY_MENU_CONSTR[4]);

	weaponButtons = {
		&B_item1, &B_item2, &B_item3, &B_item4
	};

	delegateButtons();

	shapes = { &T_Title, &B_item1, &B_item2, &B_item3, &B_item4, &B_Return };
}

void W_SelectWeapon::delegateButtons()
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

	gameInstance().modWindowName(" - Selecting Weapon...");
	setWidgetIndex(0);

	T_Title.construct();
	B_item1.construct();
	B_item2.construct();
	B_item3.construct();
	B_item4.construct();
	B_Return.construct();
}

void W_SelectWeapon::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	B_item1.tick(deltaTime);
	B_item2.tick(deltaTime);
	B_item3.tick(deltaTime);
	B_item4.tick(deltaTime);
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