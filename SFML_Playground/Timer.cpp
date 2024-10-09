#include "Timer.h"

void Timer::update(const float& deltaTime)
{
	currentTime -= deltaTime;
	if (currentTime < 0.0f)
	{
		currentTime = 0.0f;
	}
	float timeRatio = (currentTime / maxTime);
	timerBar.setSize(sf::Vector2f(barWidth * timeRatio, barHeight));
}

void Timer::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(timerBar);
}

float Timer::getCurrentTime() const
{
	return this->currentTime;
}

void Timer::setCurrentTime(const float& n)
{
	this->currentTime = std::min(n, maxTime);
}

float Timer::getMaxTime() const
{
	return this->maxTime;
}

void Timer::setMaxTime(const float& n, const bool& reset)
{
	this->maxTime = n;
	if (reset || n < currentTime) currentTime = maxTime;
}

bool Timer::isFinished() const
{
	return (currentTime <= 0.0f);
}