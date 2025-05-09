#pragma once

#include "WidgetElements.h"

#include "W_OptionsGraphics.h"
#include "W_OptionsSounds.h"


class W_Options : public InputWidget // OptionsMenu Widget ------------------------------------------
{
private:
	void delegateButtons() override;

	Button T_Title;
	Button B_Sounds;
	Button B_Graphics;
	Button B_Return;
	Button T_DevMode;

	W_OptionsSounds soundMenu;
	W_OptionsGraphics graphicMenu;
public:
	W_Options(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;
	bool isMouseOver(const bool& = false) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};