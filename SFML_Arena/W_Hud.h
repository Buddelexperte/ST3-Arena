#pragma once

#include "WidgetBase.h"

class W_Hud : public InputWidget
{
private:

public:
	W_Hud(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};