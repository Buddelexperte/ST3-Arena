#pragma once

#include "WidgetBase.h"

class W_Hud : public InputWidget
{
private:
	Button lifeBar;
	Button lifeBar_bg;

	float displayedHealth = 1.0f;

	sf::Text scoreText;
	sf::Text levelText;

	void tick_lifeBar(const float& deltaTime);

public:
	W_Hud(InputWidget*);
	void tick(const float& deltaTime) override;
	void construct() override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};