#pragma once

#include "Border.h" // Own header file
#include "GameInstance.h"

Border::Border(InputWidget* parent)
	: WidgetElement(parent)
{
	shapes = { &B_Box };
}

void Border::construct()
{
	construct(borderData);
}

void Border::construct(const RawBorder& constr)
{
	borderData = constr;

	// Setup button box
	B_Box.setSize(constr.size);
	B_Box.setFillColor(constr.color);

	setAlignment(constr.alignment);
	setPosition(constr.pos);

	RenderInfo renderInfo = { borderData.pos, borderData.size, 0.0f, { 0.f, 0.f }, borderData.color };
	setRenderInfo(renderInfo);

	tick_pos(getTickCorrection());
}

void Border::setTexture(const sf::Texture& newTexture, const bool resetTint)
{
    B_Box.setTexture(&newTexture);
    if (resetTint) B_Box.setFillColor(sf::Color::White);
}

void Border::setPosition(const sf::Vector2f& newPos)
{
    WidgetElement::setPosition(newPos);

    borderData.pos = newPos;

    // Update the buttonData position if required.
    B_Box.setPosition(newPos);
}

void Border::addPosition(const sf::Vector2f& deltaPos, const bool& bTickBased)
{
    WidgetElement::addPosition(deltaPos, bTickBased);

    if (!bTickBased)
    {
        //std::cout << "NOT TICK BASES" << std::endl;
        borderData.pos += deltaPos;
    }

    B_Box.setPosition(borderData.pos + deltaPos);
}

void Border::setRotation(const float& newRot)
{
    WidgetElement::setRotation(newRot);

    B_Box.setRotation(newRot);
}

void Border::setColor(const sf::Color& color)
{
    boxFade.fromTo.color0 = color;
    borderData.color = color;
    B_Box.setFillColor(color);
}

void Border::setSize(const sf::Vector2f& newSize)
{
}

bool Border::isMouseOver(const bool& checkForClick)
{
    const sf::Vector2f& mousePos = gameInstance().getMousePos();
    const bool bIsMouseOver = B_Box.getGlobalBounds().contains(mousePos);

    return bIsMouseOver;
}

void Border::setAlignment(const EAlignment& alignment)
{
    borderData.alignment = alignment;

    const sf::Vector2f& size = B_Box.getSize();

    switch (alignment)
    {
    case EAlignment::LEFT_TOP:
        // Anchor at the top left corner.
        B_Box.setOrigin(0.f, 0.f);
        break;
    case EAlignment::LEFT:
        // Anchor at the left edge, vertically centered.
        B_Box.setOrigin(0.f, size.y / 2.f);
        break;
    case EAlignment::LEFT_BOTTOM:
        // Anchor at the bottom left corner.
        B_Box.setOrigin(0.f, size.y);
        break;
    case EAlignment::CENTER_TOP:
        // Anchor at the center (both horizontally and vertically).
        B_Box.setOrigin(size.x / 2.f, 0.0f);
        break;
    case EAlignment::CENTER:
        // Anchor at the center (both horizontally and vertically).
        B_Box.setOrigin(size.x / 2.f, size.y / 2.f);
        break;
    case EAlignment::CENTER_BOTTOM:
        // Anchor at the center (both horizontally and vertically).
        B_Box.setOrigin(size.x / 2.f, size.y);
        break;
    case EAlignment::RIGHT_TOP:
        // Anchor at the top right corner.
        B_Box.setOrigin(size.x, 0.f);
        break;
    case EAlignment::RIGHT:
        // Anchor at the right edge, vertically centered.
        B_Box.setOrigin(size.x, size.y / 2.f);
        break;
    case EAlignment::RIGHT_BOTTOM:
        // Anchor at the bottom right corner.
        B_Box.setOrigin(size.x, size.y);
        break;
    default:
        // Fallback: center alignment.
        B_Box.setOrigin(size.x / 2.f, size.y / 2.f);
        break;
    }
}