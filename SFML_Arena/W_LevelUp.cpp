#pragma once

#include "W_LevelUp.h" // Own header file
#include "GameInstance.h"
#include "functions.h"

W_LevelUp::W_LevelUp(InputWidget* parent)
	: InputWidget(parent), 
	pf_display_1(this), pf_display_2(this), pf_display_3(this), pf_display_4(this)
{

	// Button constructors

	const std::vector<RawButton> LEVEL_UP_CONSTR = {
		// Background (bg)
		{(sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm),	sf::Vector2f{ 2000.0f, 1000.0f } * viewSizeNorm,	backgroundInterfaceColor,		0,	"",			sf::Color::Transparent,		EAlignment::CENTER,			EAlignment::CENTER_TOP},
		{(sf::Vector2f{ 0, 480.0f } * viewSizeNorm),	buttonSize,											sf::Color(255, 255, 255, 255),	24,	" S K I P",	sf::Color::Black,			EAlignment::CENTER_BOTTOM,	EAlignment::CENTER},
	};


	bg.construct(LEVEL_UP_CONSTR[0]);
	B_Skip.construct(LEVEL_UP_CONSTR[1]);

	// Perk Family constructors

	const std::vector<sf::Vector2f> FAMILY_POS = {
		(sf::Vector2f{ -690.0f, -100.0f } * viewSizeNorm),
		(sf::Vector2f{ -230.0f, -100.0f } * viewSizeNorm),
		(sf::Vector2f{ 230.0f, -100.0f } * viewSizeNorm),
		(sf::Vector2f{ 690.0f, -100.0f } * viewSizeNorm)
	};

	pf_display_1.construct(FAMILY_POS[0]);
	pf_display_2.construct(FAMILY_POS[1]);
	pf_display_3.construct(FAMILY_POS[2]);
	pf_display_4.construct(FAMILY_POS[3]);
}

void W_LevelUp::construct()
{
	setWidgetIndex(0);
}

bool W_LevelUp::isMouseOver(const bool& checkForClick)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	sf::Vector2f mousePos = gameInstance().getMousePos();

	if (B_Skip.isMouseOver(checkForClick))
	{
		if (checkForClick) onKeyEscape();
		return true;
	}

	// On no button-mouse overlap
	return false;
}

void W_LevelUp::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	bg.tick_pos(widgetOffset);
	B_Skip.tick_pos(widgetOffset);

	pf_display_1.tick_pos(widgetOffset);
	pf_display_2.tick_pos(widgetOffset);
	pf_display_3.tick_pos(widgetOffset);
	pf_display_4.tick_pos(widgetOffset);
}

InputWidget* W_LevelUp::setWidgetIndex(const int& toIndex)
{
	shapes = { &bg, &B_Skip, &pf_display_1, &pf_display_2, &pf_display_3, &pf_display_4 };

	switch ((widgetIndex = toIndex))
	{
	case 0: // SELF
		break;
	default:
		break;
	}

	return getWidgetAtIndex(widgetIndex);
}

InputWidget* W_LevelUp::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0: // SELF
		return this;
		break;
	default:
		break;
	}

	return nullptr;
}