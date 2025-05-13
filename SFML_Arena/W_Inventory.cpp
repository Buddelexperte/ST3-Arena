#pragma once

#include "W_Inventory.h" // Own header file
#include "GameInstance.h"


W_Inventory::W_Inventory(InputWidget* parent)
	: InputWidget(parent)
{


	delegateEvents();

	shapes = { };
}

void W_Inventory::delegateEvents()
{

}

void W_Inventory::construct()
{
	InputWidget::construct();

	gameInstance().modWindowName(" - Viewing inventory");
	gameInstance().setGameState(GAME_PAUSED);

	setWidgetIndex(0);
}

bool W_Inventory::onKeyTab()
{
	return onKeyEscape();
}

void W_Inventory::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
}

InputWidget* W_Inventory::setWidgetIndex(const int& toIndex)
{

	switch (widgetIndex = toIndex)
	{
	case 0: // SELF
		shapes = { };
		break;
	default:
		shapes = { };
		break;
	}

	return getWidgetAtIndex(widgetIndex);
}

InputWidget* W_Inventory::getWidgetAtIndex(const int& atIndex)
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
