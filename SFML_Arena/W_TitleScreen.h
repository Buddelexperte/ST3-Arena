#pragma once

#include "WidgetElements.h"

class W_TitleScreen : public InputWidget
{
private:
	Button T_Title;
	Button T_Highscore;
	Button B_Start;
	Button B_OptionsMenu;
	Button B_Quit;

public:
	W_TitleScreen(InputWidget* parent);
	void construct() override;
	void tick(const float& deltaTime) override;

	bool isMouseOver(const bool&) override;

	bool onKeyEscape() override;
};