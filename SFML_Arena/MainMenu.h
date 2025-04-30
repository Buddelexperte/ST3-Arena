#pragma once

#include "MainMenuSubWidgets.h" // Own header
#include "ColorFade.h"

class W_MainMenu : public InputWidget // MainMenu Widget ------------------------------------------
{
private:
	ColorFade fadeScreen;

	Button menu_title;
	Button menu_highscore;
	Button menu_startButton;
	Button menu_optionsButton;
	Button menu_quitButton;

	W_Options optionsMenu;
	W_LevelMenu levelMenu;
	W_SelectWeapon selectWeapon;

	virtual bool onKeyEscape() override;
public:
	void tick(const float& deltaTime) override;
	W_MainMenu(InputWidget*);
	void construct() override;

	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};