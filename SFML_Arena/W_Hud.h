#pragma once

#include "WidgetBase.h"

class W_Hud : public InputWidget
{
private:
	// Life bar
	Button lifeBar;
	Button lifeBar_bg;
	float displayedHealth = 1.0f;

	// Score bar
	Button scoreBar;
	Button scoreBar_bg;

	Button levelDisplay;
	unsigned int displayedLevel = 1;

public:
	W_Hud();
	void tick(const float& deltaTime) override;
	void construct() override;

	void resetLifeBar();
	void updateLifeBar();

	void resetScoreBar();
	void updateScoreBar();

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};