#pragma once

#include "MainMenuSubWidgets.h" // Own header

class W_MainMenu : public InputWidget // MainMenu Widget ------------------------------------------
{
private:
	virtual sf::Vector2f getTickCorrection() const override;

	FadeScreen fadeScreen;

	W_TitleScreen titleMenu;
	W_Options optionsMenu;
	W_LevelMenu levelMenu;
	W_SelectWeapon selectWeapon;

	virtual bool onKeyEscape() override;

	void start_openAnim() override;
	void start_closeAnim() override;
	void tick_openAnim(const float&) override;
	void tick_closeAnim(const float&) override;

public:
	void tick(const float& deltaTime) override;
	W_MainMenu(InputWidget*);
	void construct() override;

	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;

};