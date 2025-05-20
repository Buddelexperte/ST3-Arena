#pragma once

#include "WidgetElements.h"

class W_SelectWeapon : public InputWidget // Inventory Widget ------------------------------------------
{
private:
	struct WeaponUnlockInfo {
		Button* button;
		Text* label;
		std::string lockedText;
		int requiredScore;
	};

	void delegateEvents() override;

	static constexpr int NUM_WEAPONS = 4;
	static constexpr float SIZE_CAROUSEL = 1500.0f;
	static constexpr float WEAPON_DIST = SIZE_CAROUSEL / NUM_WEAPONS;

	static const inline sf::Color WEAPON_SELECT_COLOR = sf::Color(100, 100, 100, 255);

	Text T_Title;
	Button B_Pistol;
	Text T_Pistol;
	Button B_Rifle;
	Text T_Rifle;
	Button B_Rifle_Burst;
	Text T_Rifle_Burst;
	Button B_Shotgun;
	Text T_Shotgun;
	Button B_Return;
	
	// Keeping all weaponButtons in one place
	std::vector<Button*> weaponButtons;

	std::string selectedWeaponName = "";
	void tryStartGame();
	void checkForWeaponUnlock();

public:
	void tick(const float& deltaTime) override;
	W_SelectWeapon(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool&) override;

	bool onKeyEscape() override;
};