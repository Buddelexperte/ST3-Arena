#pragma once

#include "WidgetBase.h"

class W_SelectWeapon : public InputWidget // Inventory Widget ------------------------------------------
{
private:
	static constexpr unsigned int NUM_WEAPONS = 4; // Anzahl Weaffen Buttons
	float WEAPON_SPACING_X;

	Button inventory_title;
	Button item1_Button;
	Button item2_Button;
	Button item3_Button;
	Button item4_Button;
	Button return_Button;
public:
	void tick(const float& deltaTime) override;
	W_SelectWeapon(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int& newIndex) override;
};