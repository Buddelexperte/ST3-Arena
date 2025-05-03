#pragma once

#include "W_SelectWeapon.h" // Own header file
#include "GameInstance.h"

W_SelectWeapon::W_SelectWeapon(InputWidget* parent) 
	: InputWidget(parent),
	WEAPON_SPACING_X(viewHalfSize.x / NUM_WEAPONS),
	inventory_title(this), 
	item1_Button(this), item2_Button(this), item3_Button(this), item4_Button(this), 
	return_Button(this)
{
	const std::vector<RawButton> INVENTORY_MENU_CONSTR = {
		{sf::Vector2f(0.0f,						-300.0f) * unitNorm,	sf::Vector2f(100.0f, 100.0f) * unitNorm,	sf::Color::Transparent,		100,	"INVENTORY",	sf::Color::White},
		{sf::Vector2f(-1.5f * WEAPON_SPACING_X, -75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"PISTOL",		sf::Color::White},
		{sf::Vector2f(-0.5f * WEAPON_SPACING_X, -75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"SHOTGUN",		sf::Color::White},
		{sf::Vector2f(0.5f * WEAPON_SPACING_X,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"RIFLE",		sf::Color::White},
		{sf::Vector2f(1.5f * WEAPON_SPACING_X,	-75.0f) * unitNorm,		sf::Vector2f(200.0f, 200.0f) * unitNorm,	sf::Color(100, 100, 100),	24,		"BURST RIFLE",	sf::Color::White},
		{sf::Vector2f(0.0f,						150.0f) * unitNorm,		buttonSize,										sf::Color::White,			24,		"RETURN",		sf::Color::Black}
	};

	inventory_title.construct(INVENTORY_MENU_CONSTR[0]);
	item1_Button.construct(INVENTORY_MENU_CONSTR[1]);
	item2_Button.construct(INVENTORY_MENU_CONSTR[2]);
	item3_Button.construct(INVENTORY_MENU_CONSTR[3]);
	item4_Button.construct(INVENTORY_MENU_CONSTR[4]);
	return_Button.construct(INVENTORY_MENU_CONSTR[5]);
}

void W_SelectWeapon::construct()
{
	InputWidget::construct();
	setWidgetIndex(0);
}

void W_SelectWeapon::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	inventory_title.tick(deltaTime);
	item1_Button.tick(deltaTime);
	item2_Button.tick(deltaTime);
	item3_Button.tick(deltaTime);
	item4_Button.tick(deltaTime);
	return_Button.tick(deltaTime);
}

InputWidget* W_SelectWeapon::setWidgetIndex(const int& newIndex)
{
	switch (widgetIndex = newIndex)
	{
	case 0:
		shapes = { &inventory_title, &item1_Button, &item2_Button, &item3_Button, &item4_Button, &return_Button };
		break;
	default:
		shapes = { getWidgetAtIndex(widgetIndex) };
		break;
	}
	return getWidgetAtIndex(widgetIndex);
}


bool W_SelectWeapon::isMouseOver(const bool& checkForClick = false)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	if (return_Button.isMouseOver(checkForClick))
	{
		if (checkForClick)
		{
			onKeyEscape();
		}
		return true;
	}

	bool bStartGame = false;
	std::string selectedWeaponName = "NoWeapon";

	std::vector<Button> weaponButtons = {
		item1_Button, item2_Button, item3_Button, item4_Button
	};

	for (Button& button : weaponButtons)
	{
		if (button.isMouseOver(checkForClick))
		{
			if (checkForClick)
			{
				selectedWeaponName = button.getText();
				bStartGame |= true;
				break;
			}
		}
	}

	if (bStartGame)
	{
		gameInstance().getPlayer()->getInventory().setStartWeapon(selectedWeaponName);
		parent->playAnim(CLOSE_ANIM);
		return true;
	}

	return false;
}

bool W_SelectWeapon::onKeyEscape()
{
	if (isChildActive())
	{
		return getActiveChild()->onKeyEscape();
	}

	if (parent != nullptr)
	{
		// Overridden this to only go back ONE widget instead of whole MainMenu
		parent->setWidgetIndex(parent->getWidgetIndex() - 1);
		return true;
	}

	return false;
}
