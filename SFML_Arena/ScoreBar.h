#pragma once

#include "WidgetElements.h"

class ScoreBar : public WidgetElement
{
private:
	Button scoreBar;
	Button scoreBar_bg;

	Button levelDisplay;
	unsigned int displayedLevel = 0;

	void tick_bar(const float& deltaTime);

public:
	ScoreBar(InputWidget* parent);
	void tick(const float& deltaTime) override;
	void construct() override;

	void reset() override;
};