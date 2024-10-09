#include <SFML/Graphics.hpp>
#include "Button.h" // Buttons
#include "Timer.h" // TimerBar
#include "TargetController.h" // TargetSpawner and Handler
#include <vector> // Shape storage and access

// Enum for handling gameStages
enum GameState {
    GAME_ENDED = -1, // Not started or interrupted
    MENU_SCREEN = 0, // A Menu with clickable buttons
    GAME_OVER,
    GAME_LAUNCHING, // gameLoop should start and execute init functionality
    IN_GAME // gameLoop should start
} gameState;

// Globals
int highscore = 0;
float fps = 0.0f;
float deltaTime = 0.0f;
sf::Vector2f mousePos(0, 0);

void addMenuToShapes(const std::vector<sf::Drawable*>& vector, std::vector<sf::Drawable*>& shapes)
{
    for (const auto& elem : vector) shapes.push_back(elem);
}
void drawAll(sf::RenderWindow&, const std::vector<sf::Drawable*>&);
bool gameLoop(sf::RenderTarget&, std::vector<sf::Drawable*>&, Timer*, TargetController*, int&);

int main()
{
    // Set gameState for all actions done before player interaction
    gameState = GAME_ENDED;
    // Create window and save size + center
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML_Clicker", sf::Style::Fullscreen);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
    // Initiate clock for fps calculation
    sf::Clock clock;
    // Target Spawner and Handler
    TargetController* targetController = new TargetController();
    // MainMenu Button constructs as config variable
    const std::vector<ButtonConstruct> MAIN_MENU_CONSTR = {
        {windowCenter + sf::Vector2f{ 0, -300 }, sf::Vector2f{ 650, 120 }, sf::Color::Transparent, 100, "CLICKER GAME", sf::Color::White},
        {windowCenter + sf::Vector2f{ 0, -200 }, sf::Vector2f{ 100, 100 }, sf::Color::Transparent, 16, "Higscore: 0", sf::Color::White},
        {windowCenter + sf::Vector2f{ 0, 0 }, sf::Vector2f{ 300, 100 }, sf::Color::White, 24, "START", sf::Color::Black},
        {windowCenter + sf::Vector2f{ 0, 150 }, sf::Vector2f{ 300, 100 }, sf::Color::White, 24, "OPTIONS", sf::Color::Black},
        {windowCenter + sf::Vector2f{ 0, 300 }, sf::Vector2f{ 300, 100 }, sf::Color::White, 24, "QUIT", sf::Color::Black}
    };

    // Master Vector for shape storage, access and handled drawing
    std::vector<sf::Drawable*> shapes;

    // Creation of shapes and MenuVectors
    Button* menu_title = new Button(MAIN_MENU_CONSTR[0]);
    Button* menu_highscore = new Button(MAIN_MENU_CONSTR[1]);
    Button* menu_startButton = new Button(MAIN_MENU_CONSTR[2]);
    Button* menu_optionsButton = new Button(MAIN_MENU_CONSTR[3]);
    Button* menu_quitButton = new Button(MAIN_MENU_CONSTR[4]);
    const std::vector<sf::Drawable*> MAIN_MENU = { menu_title, menu_highscore, menu_startButton, menu_optionsButton, menu_quitButton };
    
    Timer* healthBar = new Timer(10.0f, windowSize.x, 100.0f, sf::Vector2f(windowCenter.x, 0.0f));
     
    // Main Menu added to viewport and gameState changed accordingly (ready for interaction)
    addMenuToShapes(MAIN_MENU, shapes);
    gameState = MENU_SCREEN;

    while (window.isOpen())
    {
        // Update viewport values
        windowSize = window.getSize();
        windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
        // Calculate fps and deltaTime based on clock
        deltaTime = clock.restart().asSeconds();
        fps = 1.0f / deltaTime;

        // If game is launching or at a later step already
        if (gameState >= GAME_LAUNCHING)
        {
            // Enter gameLoop with params
            if (gameLoop(window, shapes, healthBar, targetController, highscore));
            else // If game ended (loose, error, etc..)
            {
                // switch back to MainMenu
                gameState = MENU_SCREEN;
                menu_title->setColor(sf::Color::White, true);
                menu_highscore->setText("Highscore: " + std::to_string(highscore));
                for (const auto& elem : MAIN_MENU) shapes.push_back(elem);
            }
        }
        // Event listener
        sf::Event event;
        // Only check for events if the game started correctly and didn't (technically) end
        while (window.pollEvent(event) && gameState != GAME_ENDED)
        {
            // If close event got called, act accordingly
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            // Update global mouse position variable
            mousePos.x = (event.mouseButton.x);
            mousePos.y = (event.mouseButton.y);

            // Event Handler
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed: // Mouse input
                if (event.mouseButton.button == sf::Mouse::Left) // LMB
                {
                    // Only when in MainMenu, check for button presses
                    if (gameState == MENU_SCREEN)
                    {
                        if (menu_startButton->isMouseOver(mousePos)) // Start Button clicked
                        {
                            shapes.clear();
                            gameState = GAME_LAUNCHING;
                        }
                        if (menu_quitButton->isMouseOver(mousePos)) // Quit Button clicked
                        {
                            window.close();
                        }
                        if (menu_title->isMouseOver(mousePos)) // Title Text clicked
                        {
                            bool alreadyChanged = (menu_title->getColor(true) == sf::Color::Red);
                            menu_title->setColor((alreadyChanged ? sf::Color::White : sf::Color::Red), true);
                        }
                    }
                }
                break;
            case sf::Event::KeyPressed: // Keyboard input
                if (event.key.code == sf::Keyboard::Escape) // ESC
                {
                    // If already on MainMenu, close game
                    if (gameState == MENU_SCREEN)
                    {
                        window.close();
                        break;
                    }
                    // else, go to MainMenu
                    gameState = MENU_SCREEN;
                    shapes.clear();
                    for (const auto& elem : MAIN_MENU) shapes.push_back(elem);
                }
                break;
            default:
                break;
            }
        }
        // Clear viewport for new draw
        window.clear();
        // Draw all Drawables from shapes vector
        drawAll(window, shapes);
        // Display Draw changes
        window.display();
    }

    // Clean up dynamically allocated shapes after game closes
    for (auto shape : shapes) {
        delete shape;
    }

    return 0;
}

void drawAll(sf::RenderWindow& window, const std::vector<sf::Drawable*>& shapes) {
    for (const auto& shape : shapes) {
        window.draw(*shape);
    }
}

bool gameLoop(sf::RenderTarget& window, std::vector<sf::Drawable*>& shapes, Timer* healthBar, TargetController* targetController, int& highscore)
{
    static int hitTargets = 0;
    const float startTimer = 10.0f;
    const float minTimer = 1.0f;
    // If called from Menu, initiate game logic
    if (gameState == GAME_LAUNCHING)
    {
        // Reset values to game start values
        hitTargets = 0;
        targetController->initSpawner(window);
        healthBar->setMaxTime(startTimer, true);
        // Add Gameplay objects to shapes vector to draw them
        shapes.push_back(targetController);
        shapes.push_back(healthBar);
        gameState = IN_GAME;
    }
    // Update Gameplay objects with respectable params
    targetController->update(window);
    healthBar->update(deltaTime);
    // If any target got clicked
    if (targetController->clickedAny(mousePos))
    {
        // Increase targetsHit and change HealthBar accoridngly
        hitTargets++;
        float newMaxTime = startTimer - (float(int(hitTargets) / 3) * 0.2f);
        healthBar->setMaxTime(std::max(newMaxTime, minTimer)); // Shorten HealthBar lifespan
        healthBar->setCurrentTime(healthBar->getCurrentTime() + (healthBar->getMaxTime() / 5.0f)); // Regen so a fifth of the max lifespan
    }
    // If the HealthBar drops to 0.0
    if (healthBar->isFinished())
    {
        // Remove all shapes from vector for menu shapes
        shapes.clear();
        if (hitTargets > highscore) highscore = hitTargets;
        return false;
    }

    return true;
}