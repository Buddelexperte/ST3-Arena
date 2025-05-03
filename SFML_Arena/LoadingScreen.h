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
	Button T_LoadingScreenStatus;

	void start_openAnim() override;
	void start_closeAnim() override;
	void tick_openAnim(const float&) override;
	void tick_closeAnim(const float&) override;

public:
	void tick(const float& deltaTime) override;
	W_LoadingScreen(InputWidget*);
};