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

	PerkFamily_Tree familyTree_off;
	PerkFamily_Tree familyTree_def;
	PerkFamily_Tree familyTree_util;
	PerkFamily_Tree familyTree_sup;
	std::vector<PerkFamily_Tree*> familyTrees;

public:
	W_LevelUp(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	void reset() override;

	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};