#pragma once

#include "W_LevelUp.h" // Own header file
#include "GameInstance.h"
#include "functions.h"


W_LevelUp::W_LevelUp(InputWidget* parent)
	: InputWidget(parent), 
	bg(this), familySelect(this),
	familyTree_1(this), familyTree_2(this), familyTree_3(this), familyTree_4(this)
{
	const RawBorder CONSTR_BG = {
		sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm,
		sf::Vector2f{ 2000.0f, 1000.0f } * viewSizeNorm,
		backgroundInterfaceColor,
		EAlignment::CENTER
	};

	bg.construct(CONSTR_BG);

	delegateEvents();

	familyTree_1.construct(sf::Vector2f(0.0f, 0.0f), PerkFamily::Offensive);
	familyTree_2.construct(sf::Vector2f(0.0f, 0.0f), PerkFamily::Defensive);
	familyTree_3.construct(sf::Vector2f(0.0f, 0.0f), PerkFamily::Utility);
	familyTree_4.construct(sf::Vector2f(0.0f, 0.0f), PerkFamily::Support);
}

void W_LevelUp::delegateEvents()
{
	familySelect.onSelectFamily = [this](PerkFamily family)
		{
			switch (family)
			{
			case PerkFamily::Offensive:
				currTree = &familyTree_1;
				break;
			case PerkFamily::Defensive:
				currTree = &familyTree_2;
				break;
			case PerkFamily::Utility:
				currTree = &familyTree_3;
				break;
			case PerkFamily::Support:
				currTree = &familyTree_4;
				break;
			default:
				currTree = nullptr;
				break;
			}

			setWidgetIndex(1)->construct();
		};
}

void W_LevelUp::construct()
{
	InputWidget::construct();

	gameInstance().setGameState(GAME_PAUSED);

	setWidgetIndex(0);

	bg.construct();
	familySelect.construct();
}

bool W_LevelUp::isMouseOver(const bool& checkForClick)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	// On no button-mouse overlap
	return familySelect.isMouseOver(checkForClick);
}

bool W_LevelUp::onKeyEscape()
{
	if (parent != nullptr)
	{
		parent->setWidgetIndex(0)->construct();
		return true;
	}

	return false;
}

void W_LevelUp::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	bg.tick(deltaTime);
	familySelect.tick(deltaTime);
}

InputWidget* W_LevelUp::setWidgetIndex(const int& toIndex)
{
	shapes = { &bg };

	switch ((widgetIndex = toIndex))
	{
	case 0: // Perk Family Menu
		shapes.push_back(&familySelect);
		break;
	case 1: // Perk Family Tree
		if (currTree)
			shapes.push_back(currTree);
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
	case 1:
		return currTree;
	default:
		break;
	}

	return nullptr;
}