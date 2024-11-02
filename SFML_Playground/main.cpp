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

    // Create all widgets for later use
    W_Gameplay* GameplayRef = new W_Gameplay();
    W_MainMenu* MainMenuRef = new W_MainMenu();
    
    // Gameplay Initialization
    InputWidget* activeMenu = MainMenuRef;
    E_GameState gameState = GAME_ENDED;
    // Main Game Loop
    while (windowRef->isOpen())
    {
        // If GameState changed in earlier loop, construct new activeMenu;
        if (gameState != gameInstance.getGameState())
        {
            switch (gameState = gameInstance.getGameState())
            {
            case MENU_SCREEN:
                activeMenu = MainMenuRef;
                break;
            case GAME_PAUSED: case GAME_OVER: case GAME_LAUNCHING: case IN_GAME:
                activeMenu = GameplayRef;
                break;
            case GAME_ENDED:
                windowRef->close();
                activeMenu = nullptr;
                break;
            default:
                break;
            }
            // Initiate Menu start event
            activeMenu->construct();
        }

        // Calculate fps and deltaTime based on clock
        deltaTime = clock.restart().asSeconds();
        fps = 1.0f / deltaTime;

        // Update current Widget
        activeMenu->update(deltaTime);
        // Only check for events if the game started correctly and didn't (technically) end
        sf::Event event;
        while (windowRef->pollEvent(event) && gameInstance.getGameState() != GAME_ENDED)
        {
            // If close event got called, close window and break input wait loop
            if (event.type == sf::Event::Closed)
            {
                windowRef->close();
                break;
            }
            // Event Handler
            activeMenu->handleEvent(&event);
        }

        gameInstance.updateScreen(activeMenu);
    }

    return 0;
}