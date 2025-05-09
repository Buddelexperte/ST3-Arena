#pragma once

#include "WidgetElements.h"
#include "HealthBar.h"
#include "ScoreBar.h"

class W_Hud : public InputWidget
{
private:
	HealthBar healthBar;
	ScoreBar scoreBar;

protected:
	virtual sf::Vector2f getCorrectTickCorrection() const override;

public:
	W_Hud(InputWidget* parent);
	void tick(const float& deltaTime) override;
	void construct() override;

	void reset() override;
	void reset_health();
	void reset_score();

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};