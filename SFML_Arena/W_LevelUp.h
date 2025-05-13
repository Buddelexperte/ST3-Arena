#pragma once

#include "WidgetElements.h"
#include "PerkFamilyMenu.h"
#include "PerkFamilyTree.h"

class W_LevelUp : public InputWidget
{
private:
	void delegateEvents() override;

	Border bg;
	
	PerkFamilyMenu familySelect;

	PerkFamily_Tree* currTree = nullptr;

	PerkFamily_Tree familyTree_1;
	PerkFamily_Tree familyTree_2;
	PerkFamily_Tree familyTree_3;
	PerkFamily_Tree familyTree_4;

public:
	W_LevelUp(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	bool isMouseOver(const bool&) override;

	bool onKeyEscape() override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};