#pragma once
#include "SFML_Arena.h"


// Globals
float deltaTime = 0.0f;
float fps = 0.0f;

int main()
{
    // Initiate clock for fps + deltaTime calculation
    sf::Clock clock;
    // Game Instance init
    GI_Arena& gameInstance = GI_Arena::getInstance();
    sf::RenderWindow* windowRef = gameInstance.getWindow();

    // Create all main widgets for later use
    W_Gameplay* GameplayRef = new W_Gameplay();
    W_MainMenu* MainMenuRef = new W_MainMenu();
    
    // Gameplay Initialization
    E_GameState gameState = QUIT;
    InputWidget* activeMenu = MainMenuRef;
    // Main Game Loop
    while (windowRef->isOpen())
    {
        // If GameState changed in earlier loop, construct new activeMenu;
        if (gameState != gameInstance.getGameState())
        {
            switch (gameState = gameInstance.getGameState())
            {
            // Ending of Game or being in the MenuScreen opens up the MainMenu
            case MENU_SCREEN:
                activeMenu = MainMenuRef;
                break;
            // Any gamestate greater or equal than GameLaunching shows the GameplayMenu
            case GAME_PAUSED: case GAME_OVER: case GAME_LAUNCHING: case IN_GAME:
                activeMenu = GameplayRef;
                break;
            // Quit invalidates activeMenu to call a game crash intendedly
            case QUIT:
                activeMenu = nullptr;
                break;
            // Keeps activeMenu the same if no special rule is set
            default:
                break;
            }

            if (activeMenu == nullptr)
            {
                // End game intentionally by closing window and calling for next loop check
                windowRef->close();
                break;
            }
            // Update GameInstance's version of activeMenu to erase confusion
            gameInstance.setActiveWidget(activeMenu);
            // (Re)construct the selected widget for correct display of sub elements
            activeMenu->construct();
        }

        // Calculate fps and deltaTime based on clock
        deltaTime = clock.restart().asSeconds();
        fps = 1.0f / deltaTime;

        // Update current Widget
        activeMenu->update(deltaTime);
        // Only check for events if the game started correctly and didn't (technically) end
        sf::Event event;
        while (windowRef->pollEvent(event) && gameInstance.getGameState() != QUIT)
        {
            // If close event got called, close window and break input wait loop
            if (event.type == sf::Event::Closed)
            {
                windowRef->close();
                break;
            }
            // Event Handler
            activeMenu->handleInput(&event);
        }
        // Draw new Menu to screen through GameInstance
        gameInstance.updateScreen();
    }

    return 0;
}