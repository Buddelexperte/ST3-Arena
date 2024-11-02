#include "SFML_Arena.h"


// Globals
int SaveGame::Stored_Save = SaveGame::loadSavedData();

float fps = 0.0f;
float deltaTime = 0.0f;

int main()
{
    // Initiate clock for fps + deltaTime calculation
    sf::Clock clock;

    GI_Arena& gameInstance = GI_Arena::getInstance();
    // Set gameState for all actions done before player interaction
    sf::RenderWindow* windowRef = gameInstance.getWindow();

    // Target Spawner and Handler
    W_Gameplay* GameplayRef = new W_Gameplay();
    W_MainMenu* MainMenuRef = new W_MainMenu();
    
    // Gameplay Initialization
    InputWidget* activeMenu = MainMenuRef;
    E_GameState gameState = gameInstance.getGameState();
    // Main Game Loop
    while (windowRef->isOpen())
    {
        // Initiate Menu start event
        activeMenu->construct();

        // Calculate fps and deltaTime based on clock
        deltaTime = clock.restart().asSeconds();
        fps = 1.0f / deltaTime;

        // Update Events
        gameInstance.update();
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

        if (gameInstance.getGameStateChanges(gameState))
        {
            switch (gameState)
            {
            case GAME_ENDED:
                windowRef->close();
                break;
            case MENU_SCREEN:
                activeMenu = MainMenuRef;
                break;
            case PAUSED:
                activeMenu = GameplayRef;
                break;
            case GAME_OVER:
                activeMenu = GameplayRef;
                break;
            case GAME_LAUNCHING:
                activeMenu = GameplayRef;
                break;
            case IN_GAME:
                activeMenu = GameplayRef;
                break;
            default:
                activeMenu = nullptr;
                break;
            }
        }

        gameInstance.draw(activeMenu);
    }
    return 0;
}