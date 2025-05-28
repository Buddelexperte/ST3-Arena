#pragma once

#include "W_OptionsGraphics.h" // Own header file
#include "GameInstance.h"

W_OptionsGraphics::W_OptionsGraphics(InputWidget* parent)
	: InputWidget(parent),
	T_Title(this), bg(this),
	B_Return(this), B_Apply(this), // Title and Cancel and Apply etc
	T_Resolution(this), T_MaxFPS(this), T_VSync(this), T_Fullscreen(this), T_WParallax(this), // Texts
	T_Resolution_Val(this), T_MaxFPS_Val(this), T_VSync_Val(this), T_Fullscreen_Val(this), T_WParallax_Val(this) // Value Texts
{
	const RawText TITLE_CONSTR = {
		sf::Vector2f{ 0, -300 }, sf::Color::White, 100, "GRAPHICS", EAlignment::CENTER
	};

	const std::vector<RawButton> CONSTR = {
		// Resolution
		{sf::Vector2f{ 0.0f, -150.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "RESOLUTION",			sf::Color::White,		EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, -150.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "(Custom)",				sf::Color::White,		EAlignment::LEFT, EAlignment::RIGHT},
		// FPS
		{sf::Vector2f{ 0.0f, -50.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "FPS LIMIT",			sf::Color::White,		EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, -50.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "Max FPS",				sf::Color::White,		EAlignment::LEFT, EAlignment::RIGHT},
		// VSync
		{sf::Vector2f{ 0.0f, 50.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "VSYNC",				sf::Color::White,		EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, 50.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "Status",				sf::Color::White,		EAlignment::LEFT, EAlignment::RIGHT},
		// Fullscreen
		{sf::Vector2f{ 0.0f, 150.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "FULLSCREEN",			sf::Color::White,		EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, 150.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "Status",				sf::Color::White,		EAlignment::LEFT, EAlignment::RIGHT},
		// Menu parallax		
		{sf::Vector2f{ 0.0f, 250.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "MENU PARALLAX",		sf::Color::White,		EAlignment::RIGHT, EAlignment::LEFT},
		{sf::Vector2f{ 0.0f, 250.0f },							buttonSize * sf::Vector2f(2.0f, 1.0f),													sf::Color::Transparent,	40, "Status",				sf::Color::White,		EAlignment::LEFT, EAlignment::RIGHT},
		// Apply & Cancel
		{sf::Vector2f{ padding / 2.0f, 350.0f + padding},		buttonSize,																				disabledButtonColor,	24, "APPLY",				normalTextColor,		EAlignment::LEFT},
		{sf::Vector2f{ -padding / 2.0f, 350.0f + padding},		buttonSize,																				darkerButtonColor,		24, "RETURN",				normalTextColor,		EAlignment::RIGHT},
	};

	// Background
	const RawBorder BORDER_CONSTR = {
		sf::Vector2f{ 0, 50 },	buttonSize * sf::Vector2f(4.0f, 5.0f) + sf::Vector2f(2 * padding, padding / 2.0f),	backgroundInterfaceColor

	};

	// Title
	T_Title.construct(TITLE_CONSTR);
	// Res
	T_Resolution.construct(CONSTR[0]);
	T_Resolution_Val.construct(CONSTR[1]);
	// FPS
	T_MaxFPS.construct(CONSTR[2]);
	T_MaxFPS_Val.construct(CONSTR[3]);
	// VSync
	T_VSync.construct(CONSTR[4]);
	T_VSync_Val.construct(CONSTR[5]);
	// Fullscreen
	T_Fullscreen.construct(CONSTR[6]);
	T_Fullscreen_Val.construct(CONSTR[7]);
	// Widget Parallax
	T_WParallax.construct(CONSTR[8]);
	T_WParallax_Val.construct(CONSTR[9]);
	// Apply & Cancel
	B_Apply.construct(CONSTR[10]);
	B_Apply.setTexture(buttonTexture);
	B_Return.construct(CONSTR[11]);
	B_Return.setTexture(buttonTexture);
	// Background
	bg.construct(BORDER_CONSTR);
	TextureManager& tm = TextureManager::getInstance();
	bg.setTexture(tm.getTexturePtr(ETexture::OPTIONS_GRAPHICS_BORDER));

	delegateEvents();

	shapes = {
		&T_Title, &bg,
		&B_Return,&B_Apply,
		&T_Resolution, &T_Resolution_Val,
		&T_MaxFPS, &T_MaxFPS_Val,
		&T_VSync, &T_VSync_Val,
		&T_Fullscreen, &T_Fullscreen_Val,
		&T_WParallax, &T_WParallax_Val
	};
}

void W_OptionsGraphics::construct()
{
	InputWidget::construct();

	reset();

	// Title
	T_Title.construct();
	// Res
	T_Resolution.construct();
	T_Resolution_Val.construct();
	// FPS
	T_MaxFPS.construct();
	T_MaxFPS_Val.construct();
	// VSync
	T_VSync.construct();
	T_VSync_Val.construct();
	// Fullscreen
	T_Fullscreen.construct();
	T_Fullscreen_Val.construct();
	// Widget Parallax
	T_WParallax.construct();
	T_WParallax_Val.construct();
	// Apply & Cancel
	B_Apply.construct();
	B_Return.construct();
	// Background
	bg.construct();

}

void W_OptionsGraphics::reset()
{
	// Read back what was actually applied
	newSettings = UserSettings::getSettings();
	selectedRes = newSettings.resID;
	selectedFps = UserSettings::getFramerateIndex(newSettings.maxFPS);

	// Update the UI
	updateSettingTexts(newSettings);
	checkForDifferences();
}

void W_OptionsGraphics::delegateEvents()
{
	B_Return.onClick = [this]()
	{
		onKeyEscape();
	};

	B_Apply.onClick = [this]()
	{
		gameInstance().applySettings(newSettings);
		reset();
	};

	// Settings ----

	T_Resolution.onClick = T_Resolution_Val.onClick = [this]()
	{
		toggleResolution();
	};

	T_MaxFPS.onClick = T_MaxFPS_Val.onClick = [this]()
	{
		toggleMaxFPS();
	};

	T_VSync.onClick = T_VSync_Val.onClick = [this]()
	{
		toggleVSync();
	};

	T_Fullscreen.onClick = T_Fullscreen_Val.onClick = [this]()
	{
		toggleFullscreen();
	};

	T_WParallax.onClick = T_WParallax_Val.onClick = [this]()
	{
		toggleWidgetParallax();
	};
}

void W_OptionsGraphics::toggleResolution()
{
	// Update the selectedRes index to track the resolution selection
	selectedRes = (selectedRes + 1) % UserSettings::getNumResolutions();

	// Update the resolution to the next one in the list using the selectedRes index
	newSettings.resID = selectedRes;

	// Update the UI
	updateSettingTexts(newSettings);
	checkForDifferences();
}

void W_OptionsGraphics::toggleMaxFPS()
{
	selectedFps++;

	if (selectedFps >= UserSettings::getNumFramerates())
		selectedFps = 0;

	newSettings.maxFPS = UserSettings::getFramerate(selectedFps);

	// Update the UI
	updateSettingTexts(newSettings);
	checkForDifferences();
}

void W_OptionsGraphics::toggleVSync()
{
	newSettings.bUseVSync = !newSettings.bUseVSync;

	// Update the UI
	updateSettingTexts(newSettings);
	checkForDifferences();
}

void W_OptionsGraphics::toggleFullscreen()
{
	newSettings.bFullscreen = !newSettings.bFullscreen;

	// Update the UI
	updateSettingTexts(newSettings);
	checkForDifferences();
}

void W_OptionsGraphics::toggleWidgetParallax()
{
	newSettings.bWidgetParallax = !newSettings.bWidgetParallax;

	// Update the UI
	updateSettingTexts(newSettings);
	checkForDifferences();
}

std::string W_OptionsGraphics::formatResolutionLabel(size_t id)
{
	return UserSettings::getResolutionDesc(static_cast<int>(id));
}

void W_OptionsGraphics::updateSettingTexts(const UserSettings_Struct& s)
{
	// Resolution
	T_Resolution_Val.setText(
		formatResolutionLabel(s.resID)
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

	B_Apply.setEnabled(bChangedSome);

	if (bChangedSome)
	{
		B_Return.setText("CANCEL");
		B_Apply.setColor(darkerButtonColor);
		return;
	}

	B_Return.setText("RETURN");
	B_Apply.setColor(disabledButtonColor);
}

size_t W_OptionsGraphics::fpsToMaxFpsIndex(const unsigned int& maxFps)
{
	return size_t();
}

void W_OptionsGraphics::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);

	bg.tick(deltaTime);

	T_Title.tick(deltaTime);
	B_Return.tick(deltaTime);
	B_Apply.tick(deltaTime);

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
	// --- RESOLUTION ---
	if (T_Resolution.isMouseOver(checkForClick) || T_Resolution_Val.isMouseOver(checkForClick))
		return true;

	// --- MAX FPS ---
	if (T_MaxFPS.isMouseOver(checkForClick) || T_MaxFPS_Val.isMouseOver(checkForClick))
		return true;

	// --- VSYNC ---
	if (T_VSync.isMouseOver(checkForClick) || T_VSync_Val.isMouseOver(checkForClick))
		return true;

	// --- FULLSCREEN ---
	if (T_Fullscreen.isMouseOver(checkForClick) || T_Fullscreen_Val.isMouseOver(checkForClick))
		return true;

	// --- WIDGET PARALLAX ---
	if (T_WParallax.isMouseOver(checkForClick) || T_WParallax_Val.isMouseOver(checkForClick))
		return true;

	// --- RETURN BUTTON ---
	if (B_Return.isMouseOver(checkForClick))
		return true;

	// --- APPLY BUTTON ---
	if (B_Apply.isMouseOver(checkForClick))
		return true;

	return false;
}