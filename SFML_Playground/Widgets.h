#pragma once
#include "SFML_Arena.h"
#include "TargetController.h" // TargetSpawner and Handler
#include "Button.h" // Button class for UI
#include "Timer.h"
#include "Flashlight.h" // Timer class for game logic

class W_Options : public InputWidget
{
private:
	Button options_title;
	Button options_test;
	Button options_return;
protected:
	void windowUpdate() override;
public:
	virtual bool input_esc() override;
	void construct() override;
	W_Options(WidgetElement*);
	virtual bool isMouseOver(const bool&) override;
};

class W_LevelMenu : public InputWidget // LevelMenu Widget ------------------------------------------
{
private:
	Button levelmenu_title;
	Button level1_Button;
	Button level2_Button;
	Button level3_Button;
	Button return_Button;
protected:
public:
	bool input_esc() override;
	W_LevelMenu(WidgetElement*);
	void construct() override;
	void update(const float&) override;
	bool isMouseOver(const bool&) override;
};

class W_MainMenu : public InputWidget // MainMenu Widget ------------------------------------------
{
private:
	Button menu_title;
	Button menu_highscore;
	Button menu_startButton;
	Button menu_optionsButton;
	Button menu_quitButton;

	int widgetIndex = 0;
	InputWidget* getWidgetAtIndex(const int&);

	W_Options optionsMenu;
	W_LevelMenu levelMenu;
	virtual bool input_esc() override;
protected:
	void windowUpdate() override;
public:
	InputWidget* setWidgetIndex(const int&);
	W_MainMenu(WidgetElement*);
	void construct() override;
	void update(const float&) override;
	virtual bool isMouseOver(const bool&) override;
};

class W_Paused : public InputWidget
{
private:
	Button pause_title;
	Button pause_resumeButton;
	Button pause_optionsButton;
	Button pause_quitButton;

	W_Options optionsMenu;
	bool bOptionsOpen = false;
	void showOptions(const bool&);
protected:
	void windowUpdate() override;
public:
	bool input_esc() override;
	W_Paused(WidgetElement*);
	void update(const float&) override;
	bool isSubMenuOpen() const { return bOptionsOpen; }
	void construct() override;
	bool isMouseOver(const bool&) override;
};

class W_GameOver : public InputWidget
{
private:
	Button gameOver_title;
	Button gameOver_score;
	Button gameOver_quitButton;
public:
	void windowUpdate() override;
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
	Timer healthBar;
	Button TestBox;
	TargetController targetController;

	W_Paused pauseScreen;
	W_GameOver gameOverScreen;

	Player* player = gameInstance.getPlayer();

	Flashlight flashlightShader;

	void pause();
	void unpause();
	void lose();
	bool input_esc() override;
	bool onMouseClickR() override;
protected:
	void windowUpdate() override;
public:
	W_Gameplay(WidgetElement*);
	void construct() override;
	
	void update(const float& deltaTime) override;
	bool isMouseOver(const bool&) override;
};