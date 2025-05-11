#pragma once

#include "WidgetElements.h"

class W_SelectWeapon : public InputWidget // Inventory Widget ------------------------------------------
{
private:
	void delegateButtons() override;

	static constexpr unsigned int NUM_WEAPONS = 4; // Anzahl Weaffen Buttons
	float WEAPON_SPACING_X;

	Text T_Title;
	Button B_item1;
	Button B_item2;
	Button B_item3;
	Button B_item4;
	Button B_Return;
	
	// Keeping all weaponButtons in one place
	std::vector<Button*> weaponButtons;

	std::string selectedWeaponName = "";
	void tryStartGame();

public:
	void tick(const float& deltaTime) override;
	W_SelectWeapon(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool&) override;

	bool onKeyEscape() override;
};