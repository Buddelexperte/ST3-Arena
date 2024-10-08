#include <SFML/Graphics.hpp>
#include "Button.h"
#include <iostream>
#include <vector>
#include <string>

// Globals


void drawAll(sf::RenderWindow&, const std::vector<sf::Drawable*>&);
void gameLoop();

int main()
{
    bool bGameStarted = false;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML_Clicker", sf::Style::Fullscreen);

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
    sf::Vector2f mousePos(0, 0);

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


    while (window.isOpen())
    {
        if (bGameStarted) gameLoop;

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

            windowSize = window.getSize();
            windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };

            // Event Handler
            switch (event.type)
            {
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (startButton->isClicked(mousePos))
                    {
                        shapes.clear();
                        bGameStarted = true;
                    }
                    if (quitButton->isClicked(mousePos))
                        window.close();
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
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

void gameLoop()
{

}