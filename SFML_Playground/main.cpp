#include "SFML_Arena.h"

GI_Clicker& gameInstance = GI_Clicker::getInstance();
int SaveGame::Stored_Save = SaveGame::loadSavedData();

// Globals
float fps = 0.0f;
float deltaTime = 0.0f;
sf::Vector2f mousePos(0, 0);

int main()
{
    // Set gameState for all actions done before player interaction
    gameInstance.setGameState(GAME_ENDED);
    // Create window and save size + center
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML_Clicker", sf::Style::Fullscreen);
    sf::RenderStates states = sf::RenderStates::Default;
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
    // Initiate clock for fps calculation
    sf::Clock clock;

    // Target Spawner and Handler
    W_MainMenu* MainMenuRef = new W_MainMenu(window);
    W_Gameplay* GameplayRef = new W_Gameplay(window);
    // Main Menu added to viewport and gameState changed accordingly (ready for interaction)
    WidgetMenu* activeMenu = MainMenuRef;
    gameInstance.setGameState(MENU_SCREEN);

    E_GameState gameState = GAME_ENDED;

    // Main Game Loop
    while (window.isOpen())
    {
        // Update viewport values
        windowSize = window.getSize();
        windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
        // Calculate fps and deltaTime based on clock
        deltaTime = clock.restart().asSeconds();
        fps = 1.0f / deltaTime;

        // Event listener
        activeMenu->update(deltaTime);
        sf::Event event;
        // Only check for events if the game started correctly and didn't (technically) end
        while (window.pollEvent(event) && gameInstance.getGameState() != GAME_ENDED)
        {
            // If close event got called, act accordingly
            if (event.type == sf::Event::Closed)
            {
                window.close();
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
                    activeMenu->isInteracted(mousePos);
                }
                break;
            case sf::Event::KeyPressed: // Keyboard input
                if (event.key.code == sf::Keyboard::Escape) // ESC
                {
                    // If already on MainMenu, close game
                    if (gameInstance.getGameState() == MENU_SCREEN)
                    {
                        window.close();
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
                window.close();
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
                activeMenu = MainMenuRef;
                break;
            }
            activeMenu->init();
        }

        // Clear viewport for new draw
        window.clear();
        // Draw all Drawables from shapes vector
        activeMenu->draw(window, states);
        // Display Draw changes
        window.display();
    }
    return 0;
}