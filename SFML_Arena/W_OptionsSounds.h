#pragma once

#include "WidgetBase.h"

class W_OptionsSounds : public InputWidget // OptionSoundMenu Widget ------------------------------------------
{
private:
	Button optionsSounds_test;
	Button optionsSoundsReturn;

public:
	void tick(const float& deltaTime) override;
	void construct() override;
	W_OptionsSounds(InputWidget*);
	bool isMouseOver(const bool&) override;

};