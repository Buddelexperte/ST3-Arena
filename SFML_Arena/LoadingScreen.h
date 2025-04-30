#pragma once

#include "WidgetElements.h"
#include "ColorFade.h"

class W_LoadingScreen : public InputWidget // MainMenu Widget ------------------------------------------
{
private:
	bool bLoadedIn = false;
	static constexpr float TEXT_CHANGE_DELAY = 1.0f;

	ValueBar textChangeTimer;
	unsigned int textState = 0;

	ColorFade fadeScreen;

public:
	void tick(const float& deltaTime) override;
	W_LoadingScreen(InputWidget*);
};