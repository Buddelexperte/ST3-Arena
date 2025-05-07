#pragma once

#include "WidgetElements.h"

#include "W_OptionsGraphics.h"
#include "W_OptionsSounds.h"


class W_Options : public InputWidget // OptionsMenu Widget ------------------------------------------
{
private:
	void delegateButtons() override;

	Button T_Title;
	Button B_Graphics;
	Button B_Return;
	Button B_Sounds;

	W_OptionsSounds soundMenu;
	W_OptionsGraphics graphicMenu;
public:
	void tick(const float& deltaTime) override;
	W_Options(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool& = false) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};