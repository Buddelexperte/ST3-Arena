#pragma once

#include "WidgetBase.h"
#include "GameplaySubWidgets.h" // Own header file

class W_Gameplay : public InputWidget // Gameplay Widget ------------------------------------------
{
private:
	W_Paused pauseMenu;
	W_GameOver gameOverScreen;
	W_Inventory inventoryScreen;
	W_LevelUp levelUpScreen;
	W_Hud& hud;

	static constexpr float TILING_SCALE = 0.1f; // Larger = more tiling
	sf::Vector2f backgroundPos;
	sf::Texture backgroundTexture;
	sf::VertexArray background;  // 4 vertices for the rectangle (quad), specified in Widget Constructor

	void tick_background(const float& deltaTime);

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