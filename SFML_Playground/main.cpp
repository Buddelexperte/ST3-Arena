#include <SFML/Graphics.hpp>
#include <vector>

void drawShapes(sf::RenderWindow&, const std::vector<sf::Drawable*>&);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML_Clicker", sf::Style::Fullscreen);

    // Vector for shape storage and access
    std::vector<sf::Drawable*> shapes;

    // Creation of Shapes

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();
        // Draw Shapes
        drawShapes(window, shapes);
        // Display Draw changes
        window.display();
    }

    // Clean up dynamically allocated shapes
    for (auto shape : shapes) {
        delete shape;
    }

    return 0;
}

void drawShapes(sf::RenderWindow& window, const std::vector<sf::Drawable*>& shapes) {
    for (const auto& shape : shapes) {
        window.draw(*shape);
    }
}