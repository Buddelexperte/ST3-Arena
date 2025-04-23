#pragma once

#include "WidgetBase.h"

class W_LevelUp : public InputWidget
{
private:

	Button bg;
	Button B_Cancel;
	Button B_Submit;

public:
	W_LevelUp(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};