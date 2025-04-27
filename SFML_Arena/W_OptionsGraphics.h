#pragma once

#include "WidgetElements.h"

class W_OptionsGraphics : public InputWidget // OptionsGraphicsMenu Widget ------------------------------------------
{
private:
	Button optionsGraphics_test;
	Button optionsGraphicsReturn;

public:
	void tick(const float& deltaTime) override;
	void construct() override;
	W_OptionsGraphics(InputWidget*);
	bool isMouseOver(const bool&) override;
};