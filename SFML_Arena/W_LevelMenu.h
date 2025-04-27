#pragma once

#include "WidgetElements.h"

class W_LevelMenu : public InputWidget // LevelMenu Widget ------------------------------------------
{
private:
	Button levelmenu_title;
	Button level1_Button;
	Button level2_Button;
	Button level3_Button;
	Button return_Button;

public:
	void tick(const float& deltaTime) override;
	W_LevelMenu(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int& newIndex) override;
};