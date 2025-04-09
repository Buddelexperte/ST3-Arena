#include "Button.h"
#include "GameInstance.h"
#include "SoundManager.h"
#include "FontManager.h"

Button::Button()
	: Button(RawButton{ { 0, 0 }, { 100.f, 40.f }, sf::Color::White, 24, "Text", sf::Color::Black })
{

}

Button::Button(const RawButton& constr)
{
	construct(constr);
}

void Button::construct(const RawButton& constr)
{
	buttonData = constr;

	// Setup button box
	B_Box.setSize(constr.size);
	B_Box.setFillColor(constr.color);

	// Setup text
	T_Text.setFont(FontManager::getInstance().getFont(font));
	T_Text.setString(constr.text);
	T_Text.setCharacterSize(constr.textSize);
	T_Text.setFillColor(constr.textColor);

	// Apply alignment and position
	setAlignment(constr.alignment);
	setTextAlignment(constr.textAlignment);
	setPos(constr.pos);
}

void Button::setAlignment(const EAlignment& alignment)
{
	buttonData.alignment = alignment;

	const sf::Vector2f& size = B_Box.getSize();

	switch (alignment)
	{
	case EAlignment::LEFT:
		B_Box.setOrigin(0.f, size.y / 2.f);
		break;
	case EAlignment::RIGHT:
		B_Box.setOrigin(size.x, size.y / 2.f);
		break;
	case EAlignment::CENTER:
	default:
		B_Box.setOrigin(size.x / 2.f, size.y / 2.f);
		break;
	}
}

void Button::setTextAlignment(const EAlignment& alignment)
{
    buttonData.textAlignment = alignment;

    // Get text local bounds
    sf::FloatRect bounds = T_Text.getLocalBounds();

    // Compute vertical center correction
    float yOrigin = bounds.top + bounds.height / 2.f;
    float xOrigin = 0.f;

    // Set text origin based on its alignment
    switch (alignment)
    {
    case EAlignment::LEFT:
        xOrigin = 0.f;
        break;
    case EAlignment::RIGHT:
        xOrigin = bounds.width;
        break;
    case EAlignment::CENTER:
    default:
        xOrigin = bounds.width / 2.f;
        break;
    }
    T_Text.setOrigin(xOrigin, yOrigin);

    // Get box position and size
    sf::Vector2f boxPos = B_Box.getPosition();
    sf::Vector2f boxSize = B_Box.getSize();
    constexpr float padding = 10.f;

    // Position text within the box based on text alignment
    sf::Vector2f textPos = boxPos;
    // Box alignment affects where the origin is within the box
    // We need to adjust the text position accordingly
    switch (buttonData.alignment) // Use the BOX alignment here
    {
    case EAlignment::LEFT:
        // Box origin is at left center
        switch (alignment) // Text alignment
        {
        case EAlignment::LEFT:
            textPos.x += padding;
            break;
        case EAlignment::RIGHT:
            textPos.x += boxSize.x - padding;
            break;
        case EAlignment::CENTER:
            textPos.x += boxSize.x / 2.f;
            break;
        }
        break;

    case EAlignment::RIGHT:
        // Box origin is at right center
        switch (alignment) // Text alignment
        {
        case EAlignment::LEFT:
            textPos.x -= boxSize.x - padding;
            break;
        case EAlignment::RIGHT:
            textPos.x -= padding;
            break;
        case EAlignment::CENTER:
            textPos.x -= boxSize.x / 2.f;
            break;
        }
        break;

    case EAlignment::CENTER:
    default:
        // Box origin is at center
        switch (alignment) // Text alignment
        {
        case EAlignment::LEFT:
            textPos.x -= boxSize.x / 2.f - padding;
            break;
        case EAlignment::RIGHT:
            textPos.x += boxSize.x / 2.f - padding;
            break;
        case EAlignment::CENTER:
            // Text aligns with box center, no adjustment needed
            break;
        }
        break;
    }

    // Y position stays the same since vertical centering is consistent
    T_Text.setPosition(textPos);
}


void Button::setPos(const sf::Vector2f& newPos)
{
	buttonData.pos = newPos;
	B_Box.setPosition(newPos);
	setTextAlignment(buttonData.textAlignment);
}

void Button::addPos(const sf::Vector2f& delta)
{
	setPos(buttonData.pos + delta);
}

void Button::setSize(const sf::Vector2f& newSize)
{
    buttonData.size = newSize;
    B_Box.setSize(newSize);
    setTextAlignment(buttonData.textAlignment);
}

void Button::setText(const std::string& newText)
{
	buttonData.text = newText;
	T_Text.setString(newText);
}

void Button::setColor(const sf::Color& color, const bool& bTextColor)
{
	if (bTextColor)
	{
		buttonData.textColor = color;
		T_Text.setFillColor(color);
	}
	else
	{
		buttonData.color = color;
		B_Box.setFillColor(color);
	}
}

sf::Color Button::getColor(const bool& bTextColor) const
{
	return bTextColor ? buttonData.textColor : buttonData.color;
}

void Button::setTexture(const sf::Texture& newTexture, const bool resetTint)
{
	B_Box.setTexture(&newTexture);
	if (resetTint) B_Box.setFillColor(sf::Color::White);
}

bool Button::isMouseOver(const bool& bRegisterClick)
{
	const sf::Vector2f& mouse = gameInstance().getMousePos();
	bool hovered = B_Box.getGlobalBounds().contains(mouse);

	if (hovered)
		bRegisterClick ? onClick() : onHover();
	else
		onUnhover();

	return hovered;
}

void Button::onClick() const
{
	SoundManager& soundManager = SoundManager::getInstance();
	if (buttonData.text == "RETURN" || buttonData.text == "QUIT")
		soundManager.play(soundManager.getSound_ReturnClick());
	else
		soundManager.play(soundManager.getSound_Click());
}

void Button::onHover() { /* Optional hover logic */ }

void Button::onUnhover() { /* Optional unhover logic */ }

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(B_Box, states);
	target.draw(T_Text, states);
}
