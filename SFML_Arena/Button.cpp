#pragma once

#include "Button.h" // Own header file

#include "GameInstance.h"
#include "SoundManager.h"
#include "FontManager.h"

void Button::playButtonSound() const
{
    SoundManager& soundManager = SoundManager::getInstance();
    if (buttonData.text == "RETURN" || buttonData.text == "QUIT" || buttonData.text == "CANCEL")
        soundManager.play(soundManager.getSound_ReturnClick(), ESoundEnv::UI);
    else
        soundManager.play(soundManager.getSound_Click(), ESoundEnv::UI);
}

Button::Button(InputWidget* parent)
	: WidgetElement(parent)
{
    shapes = { &B_Box , &T_Text };
}

void Button::construct()
{
    construct(buttonData);
}

void Button::construct(const RawButton& constr)
{
    const bool bStartDisabled = !getEnabled();
    construct(constr, bStartDisabled);
}

void Button::construct(const RawButton& constr, const bool startDisabled)
{
    setEnabled(!startDisabled);

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
	setPosition(constr.pos);

	RenderInfo renderInfo = { buttonData.pos, buttonData.size, 0.0f, { 0.0f, 0.0f }, buttonData.color };
	setRenderInfo(renderInfo);

    tick_pos(getTickCorrection());

	bHovered = startDisabled; // Reset hover state to false, except if disabled to stop hovering logic
    isMouseOver(false); // Check for initial onHover events
}

// Setting Alignments -----------------------------------------------------------------------------

void Button::setAlignment(const EAlignment& alignment)
{
    buttonData.alignment = alignment;

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

void Button::setTextAlignment(const EAlignment& alignment)
{
    buttonData.textAlignment = alignment;

    // Get text local bounds.
    sf::FloatRect bounds = T_Text.getLocalBounds();

    // Compute text origin based on both horizontal and vertical alignment.
    float xOrigin = 0.f;
    float yOrigin = 0.f;

    // Determine horizontal origin.
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

    // Determine vertical origin.
    switch (alignment)
    {
    case EAlignment::LEFT_TOP:
    case EAlignment::CENTER_TOP:
    case EAlignment::RIGHT_TOP:
        // Top: use the top edge (bounds.top) as the origin.
        yOrigin = bounds.top;
        break;
    case EAlignment::LEFT_BOTTOM:
    case EAlignment::CENTER_BOTTOM:
    case EAlignment::RIGHT_BOTTOM:
        // Bottom: use the bottom edge.
        yOrigin = bounds.top + bounds.height;
        break;
    case EAlignment::LEFT:
    case EAlignment::RIGHT:
    case EAlignment::CENTER:
    default:
        // Center vertically.
        yOrigin = bounds.top + bounds.height / 2.f;
        break;
    }

    T_Text.setOrigin(xOrigin, yOrigin);

    // Get box position and size.
    sf::Vector2f boxPos = B_Box.getPosition();
    sf::Vector2f boxSize = B_Box.getSize();
    constexpr float padding = 10.f;

    // Position text within the box based on text alignment.
    // Begin with the box position.
    sf::Vector2f textPos = boxPos;

    // First adjust horizontally based on the BOX alignment.
    switch (buttonData.alignment)
    {
        // When the box is aligned on the left:
    case EAlignment::LEFT:
    case EAlignment::LEFT_TOP:
    case EAlignment::LEFT_BOTTOM:
        switch (alignment)
        {
        case EAlignment::LEFT:
        case EAlignment::LEFT_TOP:
        case EAlignment::LEFT_BOTTOM:
            textPos.x += padding;
            break;
        case EAlignment::RIGHT:
        case EAlignment::RIGHT_TOP:
        case EAlignment::RIGHT_BOTTOM:
            textPos.x += boxSize.x - padding;
            break;
        case EAlignment::CENTER:
        case EAlignment::CENTER_TOP:
        case EAlignment::CENTER_BOTTOM:
        default:
            textPos.x += boxSize.x / 2.f;
            break;
        }
        break;

        // When the box is aligned on the right:
    case EAlignment::RIGHT:
    case EAlignment::RIGHT_TOP:
    case EAlignment::RIGHT_BOTTOM:
        switch (alignment)
        {
        case EAlignment::LEFT:
        case EAlignment::LEFT_TOP:
        case EAlignment::LEFT_BOTTOM:
            textPos.x -= boxSize.x - padding;
            break;
        case EAlignment::RIGHT:
        case EAlignment::RIGHT_TOP:
        case EAlignment::RIGHT_BOTTOM:
            textPos.x -= padding;
            break;
        case EAlignment::CENTER:
        case EAlignment::CENTER_TOP:
        case EAlignment::CENTER_BOTTOM:
        default:
            textPos.x -= boxSize.x / 2.f;
            break;
        }
        break;

        // When the box is centered:
    case EAlignment::CENTER:
    case EAlignment::CENTER_TOP:
    case EAlignment::CENTER_BOTTOM:
    default:
        switch (alignment)
        {
        case EAlignment::LEFT:
        case EAlignment::LEFT_TOP:
        case EAlignment::LEFT_BOTTOM:
            textPos.x -= boxSize.x / 2.f - padding;
            break;
        case EAlignment::RIGHT:
        case EAlignment::RIGHT_TOP:
        case EAlignment::RIGHT_BOTTOM:
            textPos.x += boxSize.x / 2.f - padding;
            break;
        case EAlignment::CENTER:
        case EAlignment::CENTER_TOP:
        case EAlignment::CENTER_BOTTOM:
        default:
            // No horizontal adjustment needed.
            break;
        }
        break;
    }

    // Next adjust vertically based on the BOX alignment.
    switch (buttonData.alignment)
    {
        // When the box is aligned on the top:
    case EAlignment::LEFT_TOP:
    case EAlignment::CENTER_TOP:
    case EAlignment::RIGHT_TOP:
        switch (alignment)
        {
        case EAlignment::LEFT_TOP:
        case EAlignment::RIGHT_TOP:
            // For top aligned text, move text down by padding.
            textPos.y += padding;
            break;
        case EAlignment::LEFT_BOTTOM:
        case EAlignment::RIGHT_BOTTOM:
            // For bottom aligned text, move text up by (boxSize - padding).
            textPos.y += boxSize.y - padding;
            break;
        case EAlignment::LEFT:
        case EAlignment::RIGHT:
        case EAlignment::CENTER:
        default:
            // For center aligned text, no additional vertical offset.
            textPos.y += boxSize.y / 2.f;
            break;
        }
        break;

        // When the box is aligned on the center y:
    case EAlignment::LEFT:
    case EAlignment::CENTER:
    case EAlignment::RIGHT:
        switch (alignment)
        {
        case EAlignment::LEFT_TOP:
        case EAlignment::CENTER_TOP:
        case EAlignment::RIGHT_TOP:
            // For top aligned text, move text down by padding.
            textPos.y -= boxSize.y / 2.f - padding;
            break;
        case EAlignment::LEFT_BOTTOM:
        case EAlignment::CENTER_BOTTOM:
        case EAlignment::RIGHT_BOTTOM:
            // For bottom aligned text, move text up by (boxSize - padding).
            textPos.y += boxSize.y / 2.f - padding;
            break;
        case EAlignment::LEFT:
        case EAlignment::RIGHT:
        case EAlignment::CENTER:
        default:
            // For center aligned text, no additional vertical offset.
            break;
        }
        break;

        // When the box is aligned on the bottom:
    case EAlignment::LEFT_BOTTOM:
    case EAlignment::CENTER_BOTTOM:
    case EAlignment::RIGHT_BOTTOM:
    default:
        switch (alignment)
        {
        case EAlignment::LEFT_TOP:
        case EAlignment::CENTER_TOP:
        case EAlignment::RIGHT_TOP:
            // For top aligned text, move text down by padding.
            textPos.y -= boxSize.y - padding;
            break;
        case EAlignment::LEFT_BOTTOM:
        case EAlignment::CENTER_BOTTOM:
        case EAlignment::RIGHT_BOTTOM:
            // For bottom aligned text, move text up by (boxSize - padding).
            textPos.y -= padding;
            break;
        case EAlignment::LEFT:
        case EAlignment::RIGHT:
        case EAlignment::CENTER:
        default:
            textPos.y -= boxSize.y / 2.f;
            break;
        }
        break;
    }

    T_Text.setPosition(textPos);
}

// Other setters ----------------------------------------------------------------------------------

void Button::setPosition(const sf::Vector2f& newPos)
{
	WidgetElement::setPosition(newPos);

    buttonData.pos = newPos;

	// Update the buttonData position if required.
	B_Box.setPosition(newPos);
	setTextAlignment(buttonData.textAlignment);
}

void Button::addPosition(const sf::Vector2f& deltaPos, const bool& bTickBased)
{
	WidgetElement::addPosition(deltaPos, bTickBased);

    if (!bTickBased)
    {
        //std::cout << "NOT TICK BASED" << std::endl;
		buttonData.pos += deltaPos;
    }

    B_Box.setPosition(buttonData.pos + deltaPos);
	setTextAlignment(buttonData.textAlignment);
}

void Button::setRotation(const float& newRot)
{
	WidgetElement::setRotation(newRot);

    B_Box.setRotation(newRot);
	T_Text.setRotation(newRot);
}

void Button::setSize(const sf::Vector2f& newSize)
{
    WidgetElement::setSize(newSize);

    buttonData.size = newSize;
    B_Box.setSize(newSize);
	setAlignment(buttonData.alignment);
    setTextAlignment(buttonData.textAlignment);
}

void Button::setEnabled(const bool b)
{
    bEnabled = b;
}

void Button::setText(const std::string& newText)
{
	buttonData.text = newText;
	T_Text.setString(newText);
    setTextAlignment(buttonData.textAlignment);
}

void Button::setColor(const sf::Color& color)
{
    boxFade.fromTo.color0 = color;
	buttonData.color = color;
	B_Box.setFillColor(color);
}

sf::Color Button::getColor() const
{
	return buttonData.color;
}

void Button::setTextColor(const sf::Color& color)
{
    textFade.fromTo.color0 = color;
    buttonData.textColor = color;
    T_Text.setFillColor(color);
}

sf::Color Button::getTextColor() const
{
    return buttonData.textColor;
}

void Button::setTextSize(const unsigned int& newSize)
{
    buttonData.textSize = newSize;
    T_Text.setCharacterSize(newSize);
    setTextAlignment(buttonData.textAlignment);
}

unsigned int Button::getTextSize() const
{
    return buttonData.textSize;
}

void Button::setTexture(const sf::Texture& newTexture, const bool resetTint)
{
	B_Box.setTexture(&newTexture);
	if (resetTint) B_Box.setFillColor(sf::Color::White);
}

// Mouse Interaction ------------------------------------------------------------------------------

bool Button::isMouseOver(const bool& checkForClick)
{
    // If disabled, skip hovering logic
    if (!bEnabled)
        return false;

    const sf::Vector2f& mousePos = gameInstance().getMousePos();
    const bool isMouseOver = B_Box.getGlobalBounds().contains(mousePos);

    // If mouse not overlapping hitbox
    if (!isMouseOver)
    {
        // Unhovering, if hovered before this check
        if (bHovered)
            unhover();

        return false;
    }

    // Mouse is overlapping hitbox
     
    
    // If clicked, call onClick()
    if (checkForClick && isAnimBlockingInput())
        return false;

    if (checkForClick)
        click();

    if (bHovered == false) // Else only call for onHover() if not already hovering
    {
        hover();
    }

    return true;
}

// Animations -------------------------------------------------------------------------------------

void Button::start_onHoverAnim()
{
    static constexpr float hoverDuration = 0.2f;

    sf::Color currColor = B_Box.getFillColor(); // Maybe deviates from manually set color due to smoothing
    sf::Color hoveredColor = buttonData.color - HOVER_COLOR_DELTA;
    boxFade.reset(ColorColor(currColor, hoveredColor), hoverDuration);
}

void Button::tick_onHoverAnim(const float& deltaTime)
{
    if (boxFade.done())
        stopAnim(ON_HOVER);

    B_Box.setFillColor(boxFade.fade(deltaTime));
}

void Button::start_onUnhoverAnim()
{
    static constexpr float unhoverDuration = 0.2f;

    sf::Color currColor = B_Box.getFillColor(); // Maybe deviates from manually set color due to smoothing
    boxFade.reset(ColorColor(currColor, buttonData.color), unhoverDuration);
}

void Button::tick_onUnhoverAnim(const float& deltaTime)
{
    if (boxFade.done())
        stopAnim(ON_UNHOVER);

    B_Box.setFillColor(boxFade.fade(deltaTime));
}

void Button::start_onClickAnim()
{
    // Nothing yet
}

void Button::tick_onClickAnim(const float&)
{
    // Nothing yet
}

void Button::hover()
{
    bHovered = true;
    stopAnim(EAnimation::ON_UNHOVER);
    playAnim(EAnimation::ON_HOVER);
    if (onHover) onHover();
}

void Button::unhover()
{
    bHovered = false;
    stopAnim(EAnimation::ON_HOVER);
    playAnim(EAnimation::ON_UNHOVER);
    if (onUnhover) onUnhover();
}

void Button::click()
{
    playButtonSound();
    playAnim(EAnimation::ON_CLICK);
    if (onClick) onClick(); // Only call back to onClick of onClick delegate has been set
}