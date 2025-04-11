#pragma once

#include "GameplaySubWidgets.h" // Own header file
#include "WidgetBase.h"
#include "Background.h"

class W_Gameplay : public InputWidget // Gameplay Widget ------------------------------------------
{
private:
	W_Paused pauseMenu;
	W_GameOver gameOverScreen;
	W_Inventory inventoryScreen;
	W_LevelUp levelUpScreen;
	W_Hud& hud;

	BackgroundElement background;

	void lose();

	bool onKeyEscape() override;
	bool onKeyTab() override;
	bool onMouseClickR(sf::Event*) override;
public:
	W_Gameplay(InputWidget*);

	void tick(const float& deltaTime) override;
	void construct() override;

	bool isMouseOver(const bool&) override;
	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};