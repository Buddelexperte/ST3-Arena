#pragma once

#include "WidgetElements.h"

class W_Inventory : public InputWidget
{
private:
	void delegateEvents() override;

public:
	W_Inventory(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	bool onKeyTab() override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};