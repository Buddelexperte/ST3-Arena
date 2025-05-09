#pragma once

#include "WidgetElements.h"

class W_LoadingScreen : public InputWidget // MainMenu Widget ------------------------------------------
{
private:
	bool bLoadedIn = false;
	static constexpr float TEXT_CHANGE_DELAY = 1.0f;

	ValueBar textChangeTimer;
	unsigned int textState = 0;

	FadeScreen fadeScreen;
	Button T_LoadingScreenStatus;
	Button T_GameTitle;

	void updateStatus(const unsigned int);

	void start_openAnim() override;
	void tick_openAnim(const float&) override;
	void start_idleAnim() override;
	void tick_idleAnim(const float&) override;
	void start_closeAnim() override;
	void tick_closeAnim(const float&) override;

public:
	W_LoadingScreen(InputWidget*);
	void tick(const float& deltaTime) override;
};