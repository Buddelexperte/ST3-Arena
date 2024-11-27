#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct ButtonConstruct
{
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Color color;
	unsigned int textSize;
	std::string text;
	sf::Color textColor;
};

class Button : public sf::Drawable
{
private:
	sf::Font font; // Text font
	sf::SoundBuffer clickBuffer;
	sf::Sound clickSound;
	sf::Vector2f lastScreenCenter = { 0.0f, 0.0f };
	std::string text;
public:
	// shapes
	sf::RectangleShape B_Box;
	sf::Text T_Text;
	// Constructors
	Button() 
		: Button(ButtonConstruct({ 0, 0 }, { 100.0f, 100.0f }, sf::Color::White, 24, "Text", sf::Color::Black)) 
	{
		// Load text font from project directory
		if (!font.loadFromFile("../Content/fonts/coolvetica/coolvetica_rg.otf"))
		{
			std::cerr << "Unable to load font!!" << std::endl; // Error message for console log
		}
		// Load Click Sound from project directory
		if (!clickBuffer.loadFromFile("..Content/Sounds/Click_Low.wav"))
		{
			std::cout << "Unable to load click sound!" << std::endl;
		}
	};
	Button(const ButtonConstruct& constr) { construct(constr); }
	void construct(const ButtonConstruct&);

	void setText(const std::string&); // Set the texts content
	std::string getText() const { return text; }
	void setColor(const sf::Color&, const bool& = false); // Set the color of the text or the button fill
	sf::Color getColor(const bool& = false) const; // Get the color of the text or the button fill

	void setTexture(const sf::Texture&, const bool);
	void clearTexture() { B_Box.setTexture(nullptr); };
	sf::Texture getTexture() const { return *B_Box.getTexture(); }

	void setPos(const sf::Vector2f&); // Move the whole button to a new position
	void addPos(const sf::Vector2f&);
	sf::Vector2f getPos() const { return B_Box.getPosition(); }
	void setRot(const float& newRot) { B_Box.setRotation(newRot); }
	float getRot() const { return B_Box.getRotation(); };

	bool isMouseOver(const bool& = false); // Check if mouse is over button
	void onClick();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};