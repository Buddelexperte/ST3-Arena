#pragma once

#include "WidgetElements.h"

class W_LevelMenu : public InputWidget // LevelMenu Widget ------------------------------------------
{
private:
	struct LevelUnlockInfo {
		Button* button;
		Text* label;
		std::string lockedText;
		int requiredSeconds;
	};

	void delegateEvents() override;
	
	Text T_Title;

	Button B_Level1;
	Text T_Level1;
	Button B_Level2;
	Text T_Level2;
	Button B_Level3;
	Text T_Level3;

	Button B_Return;

	void selectLevel(const unsigned int);

	void checkForWeaponUnlock();

public:
	void tick(const float& deltaTime) override;
	W_LevelMenu(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool&) override;
};