#pragma once

#include "WidgetElements.h"

class W_LevelMenu : public InputWidget // LevelMenu Widget ------------------------------------------
{
private:
	void delegateEvents() override;
	
	Text T_Title;
	Button B_Level1;
	Button B_Level2;
	Button B_Level3;
	Button B_Return;

	void selectLevel(const unsigned int);

public:
	void tick(const float& deltaTime) override;
	W_LevelMenu(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool&) override;
};