#pragma once

#include "W_OptionsGraphics.h" // Own header file
#include "GameInstance.h"

void W_OptionsGraphics::reset()
{
	// Read back what was actually applied
	newSettings = UserSettings::getSettings();
	selectedRes = newSettings.resID;

	// Update UI to match actual applied settings
	updateSettingTexts(newSettings);

	// This will update button states based on comparison with originalSettings
	checkForDifferences();
}

W_OptionsGraphics::W_OptionsGraphics(InputWidget* parent)
	: InputWidget(parent),
	T_Title(this), B_Return(this), B_Apply(this), // Title and Cancel and Apply etc
	T_Resolution(this), T_MaxFPS(this), T_VSync(this), T_Fullscreen(this), T_WParallax(this), // Texts
	T_Resolution_Val(this), T_MaxFPS_Val(this), T_VSync_Val(this), T_Fullscreen_Val(this), T_WParallax_Val(this) // Value Texts
{
	const std::vector<RawButton> MAIN_MENU_CONSTR = {
		// Title
		{sf::Vector2f{ 0, -300 },				buttonSize,									sf::Color::Transparent,		100, "GRAPHICS",		sf::Color::White},
		// Resolution
		{sf::Vector2f{ 0.0f, -150 },			buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Green,			40, "RESOLUTION",		sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, -150 },			buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Red,				40, "2560 x 1440",		sf::Color::White, EAlignment::LEFT, EAlignment::RIGHT},
		// FPS
		{sf::Vector2f{ 0.0f, -50 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Green,			40, "FPS LIMIT",		sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, -50 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Red,				40, "144 FPS",			sf::Color::White, EAlignment::LEFT, EAlignment::RIGHT},
		// VSync
		{sf::Vector2f{ 0.0f, 50 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Green,			40, "VSYNC",			sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, 50 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Red,				40, "ON",				sf::Color::White, EAlignment::LEFT, EAlignment::RIGHT},
		// Fullscreen
		{sf::Vector2f{ 0.0f, 150 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Green,			40, "FULLSCREEN",			sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, 150 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Red,				40, "ON",				sf::Color::White, EAlignment::LEFT, EAlignment::RIGHT},
		// Menu parallax
		{sf::Vector2f{ 0.0f, 250 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Green,			40, "MENU PARALLAX",	sf::Color::White, EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, 250 },				buttonSize * sf::Vector2f(2.0f, 1.0f),		sf::Color::Red,				40, "DISABLED",			sf::Color::White, EAlignment::LEFT, EAlignment::RIGHT},
		// Apply & Cancel
		{sf::Vector2f{ 10, 350 + padding},		buttonSize,									grayedOutButtonColor,		24, "APPLY",			sf::Color::Black, EAlignment::LEFT},
		{sf::Vector2f{ -10, 350 + padding},		buttonSize,									sf::Color::White,			24, "RETURN",			sf::Color::Black, EAlignment::RIGHT}

	};

	// Title
	T_Title.construct(MAIN_MENU_CONSTR[0]);
	// Res
	T_Resolution.construct(MAIN_MENU_CONSTR[1]);
	T_Resolution_Val.construct(MAIN_MENU_CONSTR[2]);
	// FPS
	T_MaxFPS.construct(MAIN_MENU_CONSTR[3]);
	T_MaxFPS_Val.construct(MAIN_MENU_CONSTR[4]);
	// VSync
	T_VSync.construct(MAIN_MENU_CONSTR[5]);
	T_VSync_Val.construct(MAIN_MENU_CONSTR[6]);
	// Fullscreen
	T_Fullscreen.construct(MAIN_MENU_CONSTR[7]);
	T_Fullscreen_Val.construct(MAIN_MENU_CONSTR[8]);
	// Widget Parallax
	T_WParallax.construct(MAIN_MENU_CONSTR[9]);
	T_WParallax_Val.construct(MAIN_MENU_CONSTR[10]);
	// Apply & Cancel
	B_Apply.construct(MAIN_MENU_CONSTR[11]);
	B_Return.construct(MAIN_MENU_CONSTR[12]);

	shapes = {
		&T_Title,
		&B_Return,&B_Apply,
		&T_Resolution, &T_Resolution_Val,
		&T_MaxFPS, &T_MaxFPS_Val,
		&T_VSync, &T_VSync_Val,
		&T_Fullscreen, &T_Fullscreen_Val,
		&T_WParallax, &T_WParallax_Val
	};
}

std::string W_OptionsGraphics::formatResolutionLabel(size_t id)
{
	return UserSettings::getResolutionDesc(static_cast<int>(id));
}

void W_OptionsGraphics::construct()
{
	InputWidget::construct();

	reset();
}

void W_OptionsGraphics::updateSettingTexts(const UserSettings_Struct& s)
{
	// Resolution
	T_Resolution_Val.setText(
		formatResolutionLabel(selectedRes)
	);

	// Max FPS
	T_MaxFPS_Val.setText(
		std::to_string(s.maxFPS) + " FPS"
	);

	// VSync
	T_VSync_Val.setText(
		s.bUseVSync ? "ON" : "OFF"
	);

	// Widget Parallax
	T_WParallax_Val.setText(
		s.bWidgetParallax ? "ENABLED" : "DISABLED"
	);

	// Fullscreen
	T_Fullscreen_Val.setText(
		s.bFullscreen ? "YES" : "NO"
	);
}


void W_OptionsGraphics::checkForDifferences()
{
	// Check if current settings differ from ORIGINAL settings
	bChangedSome = (newSettings != UserSettings::getSettings());

	if (bChangedSome)
	{
		B_Return.setText("CANCEL");
		B_Apply.setColor(normalButtonColor);
		return;
	}

	B_Return.setText("RETURN");
	B_Apply.setColor(grayedOutButtonColor);
}

void W_OptionsGraphics::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	T_Title.tick(deltaTime);
	B_Return.tick(deltaTime);
	B_Apply.tick(deltaTime);
	// Actual options
	T_Resolution.tick(deltaTime);
	T_Resolution_Val.tick(deltaTime);
	T_MaxFPS.tick(deltaTime);
	T_MaxFPS_Val.tick(deltaTime);
	T_VSync.tick(deltaTime);
	T_VSync_Val.tick(deltaTime);
	T_Fullscreen.tick(deltaTime);
	T_Fullscreen_Val.tick(deltaTime);
	T_WParallax.tick(deltaTime);
	T_WParallax_Val.tick(deltaTime);
}

bool W_OptionsGraphics::isMouseOver(const bool& checkForClick)
{
	bool bInteracted = false;

	// --- RESOLUTION ---
	if (T_Resolution.isMouseOver(checkForClick) || T_Resolution_Val.isMouseOver(checkForClick))
	{
		bInteracted |= true;
		if (checkForClick)
		{
			// Update the selectedRes index to track the resolution selection
			selectedRes = (selectedRes + 1) % UserSettings::getNumResolutions();

			// Update the resolution to the next one in the list using the selectedRes index
			newSettings.resID = selectedRes;

			// Update the UI
			updateSettingTexts(newSettings);
			checkForDifferences();
		}
	}

	// --- MAX FPS ---
	if (T_MaxFPS.isMouseOver(checkForClick) || T_MaxFPS_Val.isMouseOver(checkForClick))
	{
		bInteracted |= true;
		if (checkForClick)
		{
			if (newSettings.maxFPS == 144)
				newSettings.maxFPS = 60;
			else if (newSettings.maxFPS == 60)
				newSettings.maxFPS = 240;
			else
				newSettings.maxFPS = 144;

			// Update the UI
			updateSettingTexts(newSettings);
			checkForDifferences();
		}
	}

	// --- VSYNC ---
	if (T_VSync.isMouseOver(checkForClick) || T_VSync_Val.isMouseOver(checkForClick))
	{
		bInteracted |= true;
		if (checkForClick)
		{
			newSettings.bUseVSync = !newSettings.bUseVSync;

			// Update the UI
			updateSettingTexts(newSettings);
			checkForDifferences();
		}
	}

	// --- FULLSCREEN ---
	if (T_Fullscreen.isMouseOver(checkForClick) || T_Fullscreen_Val.isMouseOver(checkForClick))
	{
		bInteracted |= true;
		if (checkForClick)
		{
			newSettings.bFullscreen = !newSettings.bFullscreen;

			// Update the UI
			updateSettingTexts(newSettings);
			checkForDifferences();
		}
	}

	// --- WIDGET PARALLAX ---
	if (T_WParallax.isMouseOver(checkForClick) || T_WParallax_Val.isMouseOver(checkForClick))
	{
		bInteracted |= true;
		if (checkForClick)
		{
			newSettings.bWidgetParallax = !newSettings.bWidgetParallax;

			// Update the UI
			updateSettingTexts(newSettings);
			checkForDifferences();
		}
	}

	// --- RETURN BUTTON ---
	if (B_Return.isMouseOver(checkForClick))
	{
		bInteracted |= true;
		if (checkForClick)
		{
			onKeyEscape();
		}
	}

	// --- APPLY BUTTON ---
	if (B_Apply.isMouseOver(checkForClick))
	{
		bInteracted |= true;
		if (checkForClick && bChangedSome)
		{
			// Apply settings
			gameInstance().applySettings(newSettings);

			reset();
		}
	}

	return bInteracted;
}