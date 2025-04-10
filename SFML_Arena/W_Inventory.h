#pragma once

#include "WidgetBase.h"

class W_Inventory : public InputWidget
{
private:

public:
	W_Inventory(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};