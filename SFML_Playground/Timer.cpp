#include "Timer.h" // Own header file

void Timer::update(const float& deltaTime)
{
	currentTime -= deltaTime; // Subtract deltaTime from time left
	if (currentTime < 0.0f)
	{
		currentTime = 0.0f; // Keep currentTime positive
	}
	float timeRatio = (currentTime / maxTime); // Calculate ratio from timeLeft to maxTime for progress visuality
	timerBar.setSize(sf::Vector2f(barWidth * timeRatio, barHeight)); //
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
	this->currentTime = std::min(n, maxTime); // Keep currentTime in bounds of maxTime
}

float Timer::getMaxTime() const
{
	return this->maxTime;
}

void Timer::setMaxTime(const float& n, const bool& reset)
{
	this->maxTime = n;
	if (reset || n < currentTime) currentTime = maxTime; // Sets currentTime to maxTime if requested
}

bool Timer::isFinished() const
{
	return (currentTime <= 0.0f); // TimerBar isFinished if 0.0 time is left
}