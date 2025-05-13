#pragma once

#include "WidgetElements.h"

class W_OptionsSounds : public InputWidget // OptionSoundMenu Widget ------------------------------------------
{
private:
	void delegateEvents() override;

	Text T_Title;
	Button B_Return;

	Button B_ToggleSound;
	float savedVolume = 100.0f;

	void call_muteSound(const bool bMute);
	void toggleMuteSound();

	void updateText();

public:
	void tick(const float& deltaTime) override;
	void construct() override;
	W_OptionsSounds(InputWidget*);
	bool isMouseOver(const bool&) override;

};