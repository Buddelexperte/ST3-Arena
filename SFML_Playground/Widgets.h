#pragma once
#include "SFML_Arena.h"
#include "TargetController.h" // TargetSpawner and Handler
#include "Flashlight.h"


class W_MainMenu : public WidgetMenu
{
private:
	Button* menu_title;
	Button* menu_highscore;
	Button* menu_startButton;
	Button* menu_optionsButton;
	Button* menu_quitButton;
public:
	W_MainMenu();
	void init() override;
	bool isMouseOver() override;
};

class W_Gameplay : public WidgetMenu
{
private:
	const float startTimer = 10.0f;
	const float minTimer = 1.0f;
	int hitTargets = 0;
	Timer* healthBar;
	TargetController* targetController;

	Flashlight flashlightMask;

	sf::Texture texture;
	sf::Sprite* flashlight = new sf::Sprite;
public:
	W_Gameplay();
	void init() override;
	void update(const float& deltaTime) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isMouseOver() override;
};