#include <SFML/Graphics.hpp>
#pragma once
class Timer : public sf::Drawable
{
private:
    float maxTime;
    float currentTime;
    float barWidth;
    float barHeight;
    sf::RectangleShape timerBar;
public:
    Timer(float maxTime, float barWidth, float barHeight, const sf::Vector2f& position, const sf::Color& fillColor = sf::Color::White)
        : maxTime(maxTime), currentTime(0.0f), barWidth(barWidth), barHeight(barHeight)
    {
        timerBar.setSize(sf::Vector2f(barWidth, barHeight));
        timerBar.setFillColor(fillColor);
        timerBar.setPosition(position);
        timerBar.setOrigin(barWidth / 2.0f, barHeight / 2.0f);
    }

    void update(const float&);
    void setCurrentTime(const float&);
    float getMaxTime();
    void setMaxTime(const float&);
    bool isFinished() const;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

