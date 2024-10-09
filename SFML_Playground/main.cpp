#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Timer.h"
#include "TargetController.h"
#include <iostream>
#include <vector>
#include <string>

enum GameState {
    GAME_ENDED = -1,
    MENU_SCREEN = 0,
    GAME_LAUNCHING,
    IN_GAME
} gameState;

// Globals
float fps = 0.0f;
float deltaTime = 0.0f;
sf::Vector2f mousePos(0, 0);

void drawAll(sf::RenderWindow&, const std::vector<sf::Drawable*>&);
bool gameLoop(sf::RenderTarget&, std::vector<sf::Drawable*>&, Timer*, TargetController*);

int main()
{
    gameState = GAME_ENDED;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML_Clicker", sf::Style::Fullscreen);
    sf::Clock clock;

    TargetController* targetController = new TargetController();

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };

    const std::vector<ButtonConstruct> MAIN_MENU = {
        {windowCenter + sf::Vector2f{ 0, 0 }, sf::Vector2f{ 300, 100 }, sf::Color::White, 24, "START", sf::Color::Black},
        {windowCenter + sf::Vector2f{ 0, 150 }, sf::Vector2f{ 300, 100 }, sf::Color::White, 24, "OPTIONS", sf::Color::Black},
        {windowCenter + sf::Vector2f{ 0, 300 }, sf::Vector2f{ 300, 100 }, sf::Color::White, 24, "QUIT", sf::Color::Black}
    };

    // Vector for shape storage and access
    std::vector<sf::Drawable*> shapes;

    // Creation of Shapes
    Button* startButton = new Button(MAIN_MENU[0]);
    shapes.push_back(startButton);
    Button* optionsButton = new Button(MAIN_MENU[1]);
    shapes.push_back(optionsButton);
    Button* quitButton = new Button(MAIN_MENU[2]);
    shapes.push_back(quitButton);
    Timer* healthBar = new Timer(10.0f, windowSize.x, 100.0f, sf::Vector2f(windowCenter.x, 0.0f));
    
    gameState = MENU_SCREEN;

    while (window.isOpen())
    {
        windowSize = window.getSize();
        windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };

        deltaTime = clock.restart().asSeconds();
        fps = 1.0f / deltaTime;


        if (gameState >= GAME_LAUNCHING)
        {
            if (gameLoop(window, shapes, healthBar, targetController));
            else
            {
                gameState = MENU_SCREEN;
                shapes.push_back(startButton);
                shapes.push_back(optionsButton);
                shapes.push_back(quitButton);
            }
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }

            mousePos.x = (event.mouseButton.x);
            mousePos.y = (event.mouseButton.y);

            // Event Handler
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (gameState == MENU_SCREEN)
                    {
                        if (startButton->isClicked(mousePos))
                        {
                            shapes.clear();
                            gameState = GAME_LAUNCHING;
                        }
                        if (quitButton->isClicked(mousePos))
                            window.close();
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (gameState == MENU_SCREEN)
                    {
                        window.close();
                        break;
                    }
                    gameState = MENU_SCREEN;
                    shapes.clear();
                    shapes.push_back(startButton);
                    shapes.push_back(optionsButton);
                    shapes.push_back(quitButton);
                }
                break;
            default:
                break;
            }
        }

        window.clear();
        // Draw Shapes
        drawAll(window, shapes);
        // Display Draw changes
        window.display();
    }

    std::cout << "Window closed!" << std::endl;

    // Clean up dynamically allocated shapes
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

bool gameLoop(sf::RenderTarget& window, std::vector<sf::Drawable*>& shapes, Timer* healthBar, TargetController* targetController)
{
    static int hitTargets = 0;
    const float startTimer = 10.0f;
    const float minTimer = 1.0f;

    if (gameState == GAME_LAUNCHING)
    {
        hitTargets = 0;
        targetController->initSpawner(window);
        shapes.push_back(targetController);
        healthBar->setMaxTime(startTimer, true);
        shapes.push_back(healthBar);
        gameState = IN_GAME;
    }

    targetController->update(window);
    healthBar->update(deltaTime);

    if (targetController->clickedAny(mousePos))
    {
        hitTargets++;
        healthBar->setMaxTime(std::max(startTimer - (float(int(hitTargets) / 3) * 0.2f), minTimer));
        healthBar->setCurrentTime(healthBar->getCurrentTime() + (healthBar->getMaxTime() / 5.0f));
    }

    if (healthBar->isFinished())
    {
        shapes.clear();
        return false;
    }

    return true;
}