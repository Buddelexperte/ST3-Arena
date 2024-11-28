#pragma once
#include "BaseClasses.h"
#include "Button.h"
#include "Timer.h"
#include "TargetController.h"
#include "Flashlight.h"

class W_OptionsSounds : public InputWidget // OptionSoundMenu Widget ------------------------------------------
{
private:
	Button optionsSounds_test;

protected:
	void windowUpdate() override;
public:
	void construct() override;
	W_OptionsSounds(InputWidget*);
	bool isMouseOver(const bool&) override;

};

class W_OptionsGraphics : public InputWidget // OptionsGraphicsMenu Widget ------------------------------------------
{
private:
	Button optionsGraphics_test;

protected:
	void windowUpdate() override;
public:
	void construct() override;
	W_OptionsGraphics(InputWidget*);
	bool isMouseOver(const bool&) override;
};

class W_Options : public InputWidget // OptionsMenu Widget ------------------------------------------
{
private:
	Button options_title;
	Button options_graphics;
	Button options_return;
	Button options_sounds;

	W_OptionsSounds soundMenu;
	W_OptionsGraphics graphicMenu;
protected:
	void windowUpdate() override;
public:
	W_Options(InputWidget*);
	void construct() override;
	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
	bool isMouseOver(const bool& = false) override;
};	

class W_LevelMenu : public InputWidget // LevelMenu Widget ------------------------------------------
{
private:
	Button levelmenu_title;
	Button level1_Button;
	Button level2_Button;
	Button level3_Button;
	Button return_Button;
public:
	W_LevelMenu(InputWidget*);
	void construct() override;
	void update(const float&) override;
	bool isMouseOver(const bool&) override;
	
	InputWidget* setWidgetIndex(const int& newIndex) override;
	InputWidget* getWidgetAtIndex(const int& atIndex) override;
};

class W_MainMenu : public InputWidget // MainMenu Widget ------------------------------------------
{
private:
	Button menu_title;
	Button menu_highscore;
	Button menu_startButton;
	Button menu_optionsButton;
	Button menu_quitButton;

	W_Options optionsMenu;
	W_LevelMenu levelMenu;
	virtual bool input_esc() override;
protected:
	void windowUpdate() override;
public:
	W_MainMenu(InputWidget*);
	void construct() override;
	void update(const float&) override;
	bool isMouseOver(const bool&) override;

	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
};

class W_Paused : public InputWidget
{
private:
	Button pause_title;
	Button pause_resumeButton;
	Button pause_optionsButton;
	Button pause_quitButton;

	W_Options optionsMenu;
protected:
	void windowUpdate() override;
public:
	W_Paused(InputWidget*);
	void update(const float&) override;
	bool input_esc() override;
	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
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
	W_GameOver(InputWidget*);
	void windowUpdate() override;
	void changeScore(const int&);
	bool isMouseOver(const bool&) override;
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

	W_Paused pauseMenu;
	W_GameOver gameOverScreen;

	Player* player = gameInstance.getPlayer();
	sf::Vector2f playerPos;

	Flashlight flashlightShader;

	sf::Vector2f backgroundPos;
	const float TILING_SCALE = 0.1f; // Larger = more tiling
	sf::Texture backgroundTexture;
	sf::VertexArray background;  // 4 vertices for the rectangle (quad)

	void lose();
	bool input_esc() override;
	bool onMouseClickR() override;
protected:
	void windowUpdate() override;
public:
	W_Gameplay(InputWidget*);
	void construct() override;
	void update(const float& deltaTime) override;
	bool isMouseOver(const bool&) override;
	InputWidget* setWidgetIndex(const int&) override;
	InputWidget* getWidgetAtIndex(const int&) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};