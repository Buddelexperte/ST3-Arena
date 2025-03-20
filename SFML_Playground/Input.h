#pragma once

#include <SFML/Graphics.hpp>

class IHasInput
{
protected:
	virtual sf::Keyboard::Key onKeyPressed(sf::Event* eventRef)
	{
		return eventRef->key.code;
	}
	virtual sf::Mouse::Button onMouseButtonPressed(sf::Event* eventRef)
	{
		return eventRef->mouseButton.button;
	}
	virtual sf::Mouse::Button onMouseButtonReleased(sf::Event* eventRef)
	{
		return eventRef->mouseButton.button;
	}
	virtual float onMouseScrolled(sf::Event* eventRef)
	{
		if (eventRef->mouseWheelScroll.wheel != sf::Mouse::VerticalWheel) return 0.0f;
		return eventRef->mouseWheelScroll.delta;
	}
	virtual bool onMouseClickL(sf::Event*) { return false; };
	virtual bool onMouseClickR(sf::Event*) { return false; };
	virtual bool onMouseClickM(sf::Event*) { return false; };
	virtual void onMouseDownL() {};
	virtual void onMouseDownR() {};
	virtual void onMouseDownM() {};
	virtual bool onMouseReleaseL(sf::Event*) { return false; };
	virtual bool onMouseReleaseR(sf::Event*) { return false; };
	virtual bool onMouseReleaseM(sf::Event*) { return false; };

public:
	virtual bool handleEvent(sf::Event* eventRef) { return false; };
	virtual void idleInputs() { };
	virtual bool isMouseOver(const bool& = false) { return false; }
	virtual bool input_esc() { return false; };
};