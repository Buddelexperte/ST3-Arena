#pragma once
#include "SFML_Arena.h"
#include "TargetController.h" // TargetSpawner and Handler
#include "Timer.h" // Timer class for game logic
#include "Flashlight.h"
#include "Button.h" // Button class for UI

class W_Options : public InputWidget
{
private:
	Button* options_title;
	Button* options_test;
	Button* options_return;
protected:
	virtual bool input_esc() override;
public:
	W_Options(WidgetElement*);
	virtual bool isMouseOver(const bool&) override;
};

class W_MainMenu : public InputWidget // MainMenu Widget ------------------------------------------
{
private:
	Button* menu_title;
	Button* menu_highscore;
	Button* menu_startButton;
	Button* menu_optionsButton;
	Button* menu_quitButton;

	W_Options* optionsMenu = nullptr;
	bool bOptionsOpen = false;
	void showOptions(const bool&);
protected:
	virtual bool input_esc() override;
public:
	W_MainMenu(WidgetElement*);
	void construct() override;
	virtual bool isMouseOver(const bool&) override;
};

class W_Paused : public InputWidget
{
private:
	Button* pause_title;
	Button* pause_resumeButton;
	Button* pause_optionsButton;
	Button* pause_quitButton;

	W_Options* optionsMenu = nullptr;
	bool bOptionsOpen = false;
	void showOptions(const bool&);
protected:
	virtual bool input_esc() override;
public:
	W_Paused(WidgetElement*);
	virtual bool isSubMenuOpen() { return bOptionsOpen; }
	virtual void construct() override;
	virtual bool isMouseOver(const bool&) override;
};

class W_GameOver : public InputWidget
{
private:
	Button* gameOver_title;
	Button* gameOver_score;
	Button* gameOver_quitButton;
public:
	W_GameOver(WidgetElement*);
	void changeScore(const int&);
	virtual bool isMouseOver(const bool&) override;
};

class W_Gameplay : public InputWidget // Gameplay Widget ------------------------------------------
{
private:
	const float TIMER_DEFAULT = 10.0f;
	const float minTimer = 1.0f;
	int hitTargets = 0;
	Timer* healthBar;
	TargetController* targetController;

	Player* player = gameInstance.getPlayer();

	W_Paused* pauseScreen = nullptr;
	W_GameOver* gameOverScreen = nullptr;

	Flashlight flashlightMask;

	virtual void pause();
	virtual void unpause();
	void lose();
protected:
	virtual bool input_esc() override;
public:
	W_Gameplay(WidgetElement*);
	void construct() override;
	
	void update(const float& deltaTime) override;
	virtual bool isMouseOver(const bool&) override;
};