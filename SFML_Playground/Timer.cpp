#include "Timer.h"

void Timer::update(const float& deltaTime)
{
	currentTime += deltaTime;
	float timeRatio = 1.0f - (currentTime / maxTime);
	if (timeRatio < 0.0f)
	{
		timeRatio = 0.0f;
	}
	timerBar.setSize(sf::Vector2f(barWidth * timeRatio, barHeight));
}

void Timer::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(timerBar);
}

void Timer::setCurrentTime(const float& newTime)
{
	currentTime = maxTime - newTime;
}

bool Timer::isFinished() const
{
	return (currentTime >= maxTime);
}