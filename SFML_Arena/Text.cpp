#pragma once

#include "Text.h" // Own header file

Text::Text(InputWidget* parent)
	: WidgetElement(parent)
{
	shapes = { &T_Text };
}

void Text::construct()
{
	construct(textData);
}

void Text::construct(const RawText& constr)
{
    textData = constr;

    // Setup text
    T_Text.setFont(FontManager::getInstance().getFont(font));
    T_Text.setString(constr.text);
    T_Text.setCharacterSize(constr.fontSize);
    T_Text.setFillColor(constr.color);

    // Apply alignment and position
    setAlignment(constr.alignment);

    RenderInfo renderInfo = { textData.pos, {0.0f, 0.0f}, 0.0f, {0.f, 0.f}, textData.color};
    setRenderInfo(renderInfo);

    tick_pos(getTickCorrection());
}

void Text::setPosition(const sf::Vector2f& newPos)
{
    WidgetElement::setPosition(newPos);

    textData.pos = newPos;
    T_Text.setPosition(getPosition());
}

void Text::addPosition(const sf::Vector2f& deltaPos, const bool& bTickBased)
{
    WidgetElement::addPosition(deltaPos, bTickBased);

    if (!bTickBased)
    {
        //std::cout << "NOT TICK BASED" << std::endl;
        textData.pos += deltaPos;
    }

    T_Text.setPosition(textData.pos + deltaPos);

    setAlignment(textData.alignment);
}

void Text::setRotation(const float& newRot)
{
    WidgetElement::setRotation(newRot);

    T_Text.setRotation(newRot);
}

void Text::setColor(const sf::Color& color)
{
    WidgetElement::setColor(color);
    
    textFade.fromTo.color0 = color;
    T_Text.setFillColor(color);

    textData.color = color;
}

void Text::setText(const std::string& newText)
{
    T_Text.setString(newText);
    
    textData.text = newText;
}


void Text::setTextSize(const unsigned int& newSize)
{
    T_Text.setCharacterSize(newSize);
    setAlignment(textData.alignment);
    
    textData.fontSize = newSize;
}

unsigned int Text::getTextSize() const
{
    return textData.fontSize;
}

// Setting Alignments -----------------------------------------------------------------------------

void Text::setAlignment(const EAlignment& alignment)
{
    textData.alignment = alignment;

    // Get local bounds of the sf::Text.
    sf::FloatRect bounds = T_Text.getLocalBounds();

    float xOrigin = 0.f;
    float yOrigin = 0.f;

    // Horizontal alignment
    switch (alignment)
    {
    case EAlignment::LEFT:
    case EAlignment::LEFT_TOP:
    case EAlignment::LEFT_BOTTOM:
        xOrigin = 0.f;
        break;
    case EAlignment::RIGHT:
    case EAlignment::RIGHT_TOP:
    case EAlignment::RIGHT_BOTTOM:
        xOrigin = bounds.width;
        break;
    case EAlignment::CENTER:
    case EAlignment::CENTER_TOP:
    case EAlignment::CENTER_BOTTOM:
    default:
        xOrigin = bounds.width / 2.f;
        break;
    }

    // Vertical alignment
    switch (alignment)
    {
    case EAlignment::LEFT_TOP:
    case EAlignment::CENTER_TOP:
    case EAlignment::RIGHT_TOP:
        yOrigin = bounds.top;
        break;
    case EAlignment::LEFT_BOTTOM:
    case EAlignment::CENTER_BOTTOM:
    case EAlignment::RIGHT_BOTTOM:
        yOrigin = bounds.top + bounds.height;
        break;
    case EAlignment::LEFT:
    case EAlignment::CENTER:
    case EAlignment::RIGHT:
    default:
        yOrigin = bounds.top + bounds.height / 2.f;
        break;
    }

    // Apply calculated origin
    T_Text.setOrigin(xOrigin, yOrigin);
}