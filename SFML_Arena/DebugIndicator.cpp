#pragma once

#include "DebugIndicator.h"
#include "GameInstance.h"

sf::Vector2f DebugindicatorText::getTickCorrection() const
{
    return viewTL + sf::Vector2f(viewSize.x, 0.0f);
}

DebugindicatorText::DebugindicatorText()
    : InputWidget(nullptr), T_Text(this)
{
    static const RawButton CONSTR = {
        sf::Vector2f(0.0f, 0.0f), buttonSize, sf::Color::Transparent, 24, "", sf::Color::Red, EAlignment::RIGHT_TOP, EAlignment::RIGHT_TOP
    };

    T_Text.construct(CONSTR, NOT_INTERACTABLE_FLAG);

    shapes = { &T_Text };
}

void DebugindicatorText::construct()
{
    InputWidget::construct();

    bool freshIsDebug = gameInstance().getIsDebugMode();
    if (bIsDebug != freshIsDebug)
    {
        bIsDebug = freshIsDebug;
        T_Text.setText( bIsDebug ? DEBUG_TEXT : "" );
    }

    T_Text.construct();
}

void DebugindicatorText::tick(const float& deltaTime)
{
    InputWidget::tick(deltaTime);

    T_Text.tick(deltaTime);
}
