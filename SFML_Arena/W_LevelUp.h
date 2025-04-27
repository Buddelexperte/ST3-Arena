#pragma once

#include "WidgetElements.h"
#include "PerkFamilyDisplay.h" // Component

class W_LevelUp : public InputWidget
{
private:

	Button bg;
	Button B_Skip;

	PerkFamily_Display pf_display_1;
	PerkFamily_Display pf_display_2;
	PerkFamily_Display pf_display_3;
	PerkFamily_Display pf_display_4;

public:
	W_LevelUp(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};