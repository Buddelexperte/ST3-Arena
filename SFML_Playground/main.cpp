#include "SFML_Arena.h"


// Globals
int SaveGame::Stored_Save = SaveGame::loadSavedData();

float fps = 0.0f;
float deltaTime = 0.0f;
sf::Vector2f mousePos(0, 0);

int main()
{
    GI_Arena& gameInstance = GI_Arena::getInstance();
    // Set gameState for all actions done before player interaction
    sf::RenderWindow* windowRef = gameInstance.getWindow();
    gameInstance.setGameState(GAME_ENDED);
    // Initiate clock for fps calculation
    sf::Clock clock;

    // Target Spawner and Handler
    W_MainMenu* MainMenuRef = new W_MainMenu();
    W_Gameplay* GameplayRef = new W_Gameplay();


    // Main Menu added to viewport and gameState changed accordingly (ready for interaction)
    WidgetMenu* activeMenu = MainMenuRef;
    gameInstance.setGameState(MENU_SCREEN);

    // Main Game Loop
    E_GameState gameState = gameInstance.getGameState();
    while (windowRef->isOpen())
    {
        activeMenu->init();

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
            // If close event got called, act accordingly
            if (event.type == sf::Event::Closed)
            {
                windowRef->close();
                break;
            }

            // Update global mouse position variable
            mousePos.x = event.mouseButton.x;
            mousePos.y = event.mouseButton.y;

            // Event Handler
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed: // Mouse input
                if (event.mouseButton.button == sf::Mouse::Left) // LMB
                {
                    activeMenu->isMouseOver();
                }
                break;
            case sf::Event::KeyPressed: // Keyboard input
                if (event.key.code == sf::Keyboard::Escape) // ESC
                {
                    // If already on MainMenu, close game
                    if (gameInstance.getGameState() == MENU_SCREEN)
                    {
                        windowRef->close();
                        break;
                    }
                    // else, go to MainMenu
                    gameInstance.setGameState(MENU_SCREEN);
                }
                break;
            default:
                break;
            }
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