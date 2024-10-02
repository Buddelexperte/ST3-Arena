#include <SFML/Graphics.hpp>
#include "Button.h"
#include <vector>
#include <string>

// Globals
sf::Vector2f mousePos(0, 0);

void drawAll(sf::RenderWindow&, const std::vector<sf::Drawable*>&);
void eventHandler(const sf::Event::EventType& eventRef)
{
    switch (eventRef)
    {
    case sf::Event::MouseButtonPressed:
        break;
    default:
        break;
    }return;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML_Clicker", sf::Style::Fullscreen);

    // Vector for shape storage and access
    std::vector<sf::Drawable*> shapes;

    // Creation of Shapes
    Button* quitButton = new Button((200.0f, 200.0f), 100, 100, "Hallo", sf::Color::Green);
    shapes.push_back(quitButton);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            mousePos.x = (event.mouseButton.x); 
            mousePos.y = (event.mouseButton.y);

            eventHandler(event.type);
        }
        
        window.clear();
        // Draw Shapes
        drawAll(window, shapes);
        // Display Draw changes
        window.display();
    }

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