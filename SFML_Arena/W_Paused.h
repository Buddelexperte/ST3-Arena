#pragma once

#include "WidgetElements.h"
#include "Options.h" // Component

class W_Paused : public InputWidget
{
private:
	void delegateButtons() override;

	sf::Vector2f getCorrectTickCorrection() const override;

	Button pause_title;
	Button B_Resume;
	Button B_OpenOptions;
	Button B_Quit;

	W_Options optionsMenu;
public:
	void tick(const float& deltaTime) override;
	W_Paused(InputWidget*);
	void construct() override;
	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};