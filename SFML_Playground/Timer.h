#pragma once
#include "WidgetBase.h"

class Timer : public WidgetElement
{
private:
    sf::RectangleShape timerBar;
    float maxTime; // Time until timerBar isFinished
    float currentTime; // Time left until timerBar isFinished
    float barWidth; // Length of timerBar
    float barHeight; // Height of timerBar
public:
    Timer(float maxTime, float barWidth, float barHeight, const sf::Color& fillColor = sf::Color::White)
        : maxTime(maxTime), currentTime(maxTime), barWidth(barWidth), barHeight(barHeight), WidgetElement(parent)
    {
        timerBar.setSize(sf::Vector2f(barWidth, barHeight));
        timerBar.setFillColor(fillColor);
        timerBar.setOrigin(barWidth / 2.0f, barHeight / 2.0f); // Center timerBar on viewport
    }

    void tick(const float&) override; // update timeLeft using deltaTime
    float getCurrentTime() const; // get TimeLeft
    void setCurrentTime(const float&); // set MaxTime
    float getMaxTime() const; // get MaxTime
    void setMaxTime(const float&, const bool& = false); // set MaxTime
    bool isFinished() const; // is timeLeft <= 0.0

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};