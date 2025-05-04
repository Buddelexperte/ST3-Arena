#pragma once

#include "GameplaySubWidgets.h" // Own header file

class W_Gameplay : public InputWidget // Gameplay Widget ------------------------------------------
{
private:
	int startAnimPhase = -1;
	static constexpr float START_DELAY = 1.0f;
	ValueBar startDelay;

	Button loadingPlaceholder;

	W_Paused pauseMenu;
	W_GameOver gameOverScreen;
	W_Inventory inventoryScreen;
	W_LevelUp levelUpScreen;
	W_Hud& hud;

	BackgroundElement background;

	ColorFade fadeScreen;

	void start_openAnim() override;
	void start_closeAnim() override;
	void tick_openAnim(const float&) override;
	void tick_closeAnim(const float&) override;

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