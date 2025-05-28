#pragma once

#include "W_LevelUp.h" // Own header file
#include "GameInstance.h"
#include "functions.h"


W_LevelUp::W_LevelUp(InputWidget* parent)
	: InputWidget(parent), 
	bg(this), familySelect(this),
	familyTree_off(this), familyTree_def(this), familyTree_util(this), familyTree_sup(this)
{
	const RawBorder CONSTR_BG = {
		sf::Vector2f{ 0.0f, 0.0f } * viewSizeNorm, sf::Vector2f{ 2000.0f, 1000.0f } * viewSizeNorm, backgroundInterfaceColor, EAlignment::CENTER
	};

	TextureManager& tm = TextureManager::getInstance();
	bg.construct(CONSTR_BG);
	bg.setTexture(tm.getTexturePtr(ETexture::LEVEL_UP_BORDER));

	delegateEvents();

	familyTrees = {
		&familyTree_off, &familyTree_def, &familyTree_util, &familyTree_sup
	};

	for (PerkFamily_Tree* tree : familyTrees)
	{
		tree->setPosition(bg.getPosition());
		tree->setSize(bg.getSize());
	}
}

void W_LevelUp::delegateEvents()
{
	familySelect.onSelectFamily = [this](PerkFamily family)
		{
			switch (family)
			{
			case PerkFamily::Offensive:
				currTree = &familyTree_off;
				break;
			case PerkFamily::Defensive:
				currTree = &familyTree_def;
				break;
			case PerkFamily::Utility:
				currTree = &familyTree_util;
				break;
			case PerkFamily::Support:
				currTree = &familyTree_sup;
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

	familyTree_off.construct(PerkFamily::Offensive);
	familyTree_def.construct(PerkFamily::Defensive);
	familyTree_util.construct(PerkFamily::Utility);
	familyTree_sup.construct(PerkFamily::Support);

	bg.construct();
	familySelect.construct();
}

void W_LevelUp::reset()
{
	for (PerkFamily_Tree* tree : familyTrees)
	{
		tree->reset();
	}
}

bool W_LevelUp::isMouseOver(const bool& checkForClick)
{
	if (isChildActive())
		return getActiveChild()->isMouseOver(checkForClick);

	// On no button-mouse overlap
	return familySelect.isMouseOver(checkForClick);
}

void W_LevelUp::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	bg.tick(deltaTime);
	familySelect.tick(deltaTime);
	// Family trees
	for (PerkFamily_Tree* tree : familyTrees)
	{
		tree->tick(deltaTime);
	}
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