#pragma once
#include "SFML_Arena.h"
#include "TargetController.h" // TargetSpawner and Handler
#include "Flashlight.h"

class W_MainMenu : public InputWidget
{
private:
	Button* menu_title;
	Button* menu_highscore;
	Button* menu_startButton;
	Button* menu_optionsButton;
	Button* menu_quitButton;
protected:
	virtual sf::Keyboard::Key keyboardInput(sf::Event* eventRef) override;
public:
	W_MainMenu();
	void construct() override;
	virtual bool isMouseOver() override;
};

class W_Gameplay : public InputWidget
{
private:
	const float startTimer = 10.0f;
	const float minTimer = 1.0f;
	int hitTargets = 0;
	Timer* healthBar;
	TargetController* targetController;

	Flashlight flashlightMask;
protected:
	virtual sf::Keyboard::Key keyboardInput(sf::Event* eventRef) override;
public:
	W_Gameplay();
	void construct() override;
	void update(const float& deltaTime) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual bool isMouseOver() override;
};