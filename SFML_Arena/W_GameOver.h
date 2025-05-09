#pragma once

#include "WidgetElements.h"
#include "SaveGame.h"

class W_GameOver : public InputWidget
{
private:
	void delegateButtons() override;

	Button T_Title;
	Button T_Score;
	Button T_Kills;
	Button B_Quit;

	sf::Vector2f getTickCorrection() const override;
public:
	W_GameOver(InputWidget*);
	void construct() override;
	void tick(const float& deltaTime) override;
	void updateStats(const SaveGame_Struct&);
	bool isMouseOver(const bool&) override;

	bool onKeyEscape() override;
};