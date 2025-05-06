#pragma once

#include "WidgetElements.h"

class W_OptionsGraphics : public InputWidget // OptionsGraphicsMenu Widget ------------------------------------------
{
private:
	Button optionsGraphics_test;
	Button optionsGraphics_Resolution;
	Button optionsGraphics_FPSLimit;
	Button optionsGraphics_Vsync;
	Button optionsGraphics_MenuParallax;
	Button optionsGraphics_Apply;
	Button optionsGraphics_return;

public:
	void tick(const float& deltaTime) override;
	void construct() override;
	W_OptionsGraphics(InputWidget*);
	bool isMouseOver(const bool&) override;
};