#pragma once

#include "WidgetElements.h" 

class HealthBar : public WidgetElement
{
private:
	// Life bar
	const float maxHealthBarWidth = 650.0f * unitNorm.x;
	Button lifeBar;
	Button lifeBar_bg;
	float displayedHealth = 0.0f;

	void tick_bar(const float& deltaTime);

public:
	HealthBar(InputWidget* parent);
	void tick(const float& deltaTime) override;
	void construct() override;

	void reset() override;
};