#pragma once

#include "WidgetElements.h"

class W_OptionsSounds : public InputWidget // OptionSoundMenu Widget ------------------------------------------
{
private:
	Button optionsSounds_test;
	Button optionsSounds_return;

	Button optionsSounds_soundAus;
	void soundAus();

public:
	void tick(const float& deltaTime) override;
	void construct() override;
	W_OptionsSounds(InputWidget*);
	bool isMouseOver(const bool&) override;



};