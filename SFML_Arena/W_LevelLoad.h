#pragma once

#include "WidgetElements.h"

class W_LevelLoad : public InputWidget
{
private:
	sf::Vector2f getCorrectTickCorrection() const override;
	// Background
	FadeScreen bg;
	// Title
	Button T_Title;
	ColorFade textFade;
	// Overlay fadeScreen
	FadeScreen fadeScreen;

	void start_openAnim() override;
	void tick_openAnim(const float&) override;
	void start_closeAnim() override;
	void tick_closeAnim(const float&) override;

public:
	W_LevelLoad(InputWidget* parent);
	void construct() override;
	void tick(const float&) override;
};