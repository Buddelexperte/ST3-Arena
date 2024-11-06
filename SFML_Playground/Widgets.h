#pragma once
#include "SFML_Arena.h"
#include "TargetController.h" // TargetSpawner and Handler
#include "Flashlight.h"

class W_Options : public InputWidget
{
private:
	Button* options_return;
	Button* options_title;
	Button* options_test;
public:
	W_Options();
	virtual bool isMouseOver() override { return false; };
};

class W_MainMenu : public InputWidget
{
private:
	Button* menu_title;
	Button* menu_highscore;
	Button* menu_startButton;
	Button* menu_optionsButton;
	Button* menu_quitButton;

	W_Options optionsMenu;
	bool bOptionsOpen = false;
protected:
	virtual sf::Keyboard::Key keyboardInput(sf::Event* eventRef) override;
public:
	W_MainMenu();
	void construct() override;
	virtual bool isMouseOver() override;
};

class W_Paused : public InputWidget
{
private:
	Button* pause_title;
	Button* pause_resumeButton;
	Button* pause_optionsButton;
	Button* pause_quitButton;
public:
	W_Paused();
	virtual bool isMouseOver() override;
};

class W_GameOver : public InputWidget
{
private:
	Button* gameOver_title;
	Button* gameOver_score;
	Button* gameOver_quitButton;
public:
	W_GameOver(const int& currScore);
	void changeScore(const int& currScore);
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

	bool bPaused = false;
	W_Paused pauseScreen;
	W_GameOver gameOverScreen;

	Flashlight flashlightMask;

	virtual void pause();
	virtual void unpause();
	void lose();
protected:
	virtual sf::Keyboard::Key keyboardInput(sf::Event* eventRef) override;
	virtual bool input_esc() override;
public:
	W_Gameplay();
	void construct() override;
	
	void update(const float& deltaTime) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual bool isMouseOver() override;
};