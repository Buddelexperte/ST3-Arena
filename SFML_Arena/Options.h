#pragma once

#include "WidgetBase.h"
#include "W_OptionsGraphics.h"
#include "W_OptionsSounds.h"


class W_Options : public InputWidget // OptionsMenu Widget ------------------------------------------
{
private:
	Button options_title;
	Button options_graphics;
	Button options_return;
	Button options_sounds;

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