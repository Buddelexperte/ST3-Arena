#pragma once

#include "WidgetElements.h"
#include "Settings.h"

// Forward declaration
class GameInstance;

class W_OptionsGraphics : public InputWidget
{
private:
    void delegateEvents() override;
    // Helper methods
    std::string formatResolutionLabel(size_t id);
    void updateSettingTexts(const UserSettings_Struct& s);
    void checkForDifferences();

    size_t fpsToMaxFpsIndex(const unsigned int& maxFps);

    // Settings state
    UserSettings_Struct newSettings;    // Settings with pending changes

    size_t selectedRes = -1;            // Selected resolution ID (starts at 'native' = -1)
    size_t selectedFps = 0;             // Starts at lowest value
    bool bChangedSome = false;          // Flag to track if any settings changed


    // UI Components
    Border bg;
    Text T_Title;
    Button B_Return;
    Button B_Apply;

    // Settings labels
    Button T_Resolution;
    Button T_MaxFPS;
    Button T_VSync;
    Button T_Fullscreen;
    Button T_WParallax;

    // Settings values
    Button T_Resolution_Val;
    Button T_MaxFPS_Val;
    Button T_VSync_Val;
    Button T_Fullscreen_Val;
    Button T_WParallax_Val;

    virtual void reset() override;

    void toggleResolution();
    void toggleMaxFPS();
    void toggleVSync();
    void toggleFullscreen();
    void toggleWidgetParallax();

public:
    W_OptionsGraphics(InputWidget* parent = nullptr);

    // Core widget methods
    virtual void construct() override;
    virtual void tick(const float& deltaTime) override;
    virtual bool isMouseOver(const bool& checkForClick) override;

};