#pragma once

#include "WidgetElements.h"

class ScoreBar : public WidgetElement
{
private:
	const float maxScoreBarWidth = viewSize.x / 3.0f ;
	Button scoreBar;
	Button scoreBar_bg;
	Button points;
	int displayedPoints = 0;

	Button levelDisplay;
	int displayedLevel = 0;

	void tick_bar(const float& deltaTime);

public:
	ScoreBar(InputWidget* parent);
	void tick(const float& deltaTime) override;
	void construct() override;

	void reset() override;
};