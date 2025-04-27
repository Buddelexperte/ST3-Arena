#pragma once

#include "WidgetElements.h"
#include "SaveGame.h"

class W_GameOver : public InputWidget
{
private:
	Button gameOver_title;
	Button gameOver_score;
	Button gameOver_kills;
	Button gameOver_quitButton;
public:
	W_GameOver(InputWidget*);
	void construct() override;
	void tick(const float& deltaTime) override;
	void updateStats(const SaveGame_Struct&);
	bool isMouseOver(const bool&) override;
};