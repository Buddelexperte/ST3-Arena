#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Timer.h"
#include "TargetController.h"
#include <iostream>
#include <vector>
#include <string>

// Globals
float fps = 0.0f;
float deltaTime = 0.0f;
sf::Vector2f mousePos(0, 0);

void drawAll(sf::RenderWindow&, const std::vector<sf::Drawable*>&);
bool gameLoop(std::vector<sf::Drawable*>&, Timer*, TargetController*);

int main()
{
    bool bGameStarted = false;

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


    while (window.isOpen())
    {
        windowSize = window.getSize();
        windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };

        deltaTime = clock.restart().asSeconds();
        fps = 1.0f / deltaTime;


        if (bGameStarted)
        {
            if (gameLoop(shapes, healthBar, targetController));
            else
            {
                bGameStarted = false;
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
                    if (!bGameStarted)
                    {
                        if (startButton->isClicked(mousePos))
                        {
                            shapes.clear();
                            bGameStarted = true;
                        }
                        if (quitButton->isClicked(mousePos))
                            window.close();
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (!bGameStarted)
                    {
                        window.close();
                        break;
                    }
                    bGameStarted = false;
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

bool gameLoop(std::vector<sf::Drawable*>& shapes, Timer* healthBar, TargetController* targetController)
{
    static bool doInit = true;
    static float deltaScale = 1.0f;
    if (doInit)
    {
        shapes.push_back(targetController);
        healthBar->setCurrentTime(10.0f);
        shapes.push_back(healthBar);
        doInit = false;
    }

    targetController->update(deltaTime * deltaScale);
    healthBar->update(deltaTime);
    if (targetController->clickedAny(mousePos))
    {
        healthBar->setCurrentTime(healthBar->getMaxTime());
        deltaScale *= 1.001;
    }

    if (healthBar->isFinished())
    {
        shapes.clear();
        targetController->clear();
        doInit = true;
        return false;
    }

    return true;
}