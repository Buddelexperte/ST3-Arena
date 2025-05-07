#pragma once

#include "WidgetElements.h"

class W_OptionsSounds : public InputWidget // OptionSoundMenu Widget ------------------------------------------
{
private:
	Button T_Title;
	Button B_Return;

	Button B_ToggleSound;
	float savedVolume = 100.0f;
	void toggleMuteSound();

public:
	void tick(const float& deltaTime) override;
	void construct() override;
	W_OptionsSounds(InputWidget*);
	bool isMouseOver(const bool&) override;

};