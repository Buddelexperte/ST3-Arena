#pragma once

#include "W_Inventory.h" // Own header file
#include "GameInstance.h"


W_Inventory::W_Inventory(InputWidget* parent)
	: InputWidget(parent),
	bg(this), T_Title(this)
{
	const std::vector<RawBorder> BORDER_CONSTR = {
		{sf::Vector2f(0.0f, 0.0f), sf::Vector2f(viewSize.x, 1000.0f * viewSizeNorm.y), backgroundInterfaceColor, EAlignment::CENTER}
	};

	const std::vector<RawText> TEXT_CONSTR = {
		{sf::Vector2f((-viewHalfSize.x + padding), ((-500.0f * viewSizeNorm.y) + padding)), normalTextColor, 48, "INVENTORY", EAlignment::LEFT_TOP}
	};

	TextureManager& tm = TextureManager::getInstance();

	bg.construct(BORDER_CONSTR[0]);
	bg.setTexture(tm.getTexturePtr(ETexture::LEVEL_UP_BORDER));

	T_Title.construct(TEXT_CONSTR[0]);

	delegateEvents();
}

void W_Inventory::delegateEvents()
{

}

void W_Inventory::construct()
{
	InputWidget::construct();

	gameInstance().addToDefaultWindowName(" - Viewing inventory");
	gameInstance().setGameState(GAME_PAUSED);

	setWidgetIndex(0);

	bg.construct();
	T_Title.construct();
}

bool W_Inventory::onKeyTab()
{
	return onKeyEscape();
}

void W_Inventory::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	bg.tick(deltaTime);
	T_Title.tick(deltaTime);
}

InputWidget* W_Inventory::setWidgetIndex(const int& toIndex)
{
	shapes = { &bg, &T_Title };

	switch (widgetIndex = toIndex)
	{
	case 0: // SELF
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
