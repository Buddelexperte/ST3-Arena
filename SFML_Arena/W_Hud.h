#pragma once

#include "WidgetElements.h"

class W_Hud : public InputWidget
{
private:
	// Life bar
	static constexpr float maxHealthBarWidth = 650.0f;
	Button lifeBar;
	Button lifeBar_bg;
	float displayedHealth = 0.0f;

	// Score bar
	Button scoreBar;
	Button scoreBar_bg;

	Button levelDisplay;
	unsigned int displayedLevel = 0;

protected:
	virtual sf::Vector2f getCorrectTickCorrection() const override;

public:
	W_Hud(InputWidget* parent);
	void tick(const float& deltaTime) override;
	void construct() override;

	void resetLifeBar();
	void updateLifeBar();

	void resetScoreBar();
	void updateScoreBar();

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};