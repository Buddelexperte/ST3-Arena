#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "SoundManager.h"

class IHasInput
{
protected:
	virtual sf::Keyboard::Key onKeyPressed(sf::Event* eventRef)
	{
		const sf::Keyboard::Key keyboardInput = eventRef->key.code;

		switch (keyboardInput)
		{
		case sf::Keyboard::Escape:
		{
			// If no sub widget open, open optionsMenu
			SoundManager& soundManager = SoundManager::getInstance();
			soundManager.play(soundManager.getSound_ReturnClick(), ESoundEnv::UI);
			onKeyEscape();
			break;
		}
		case sf::Keyboard::Tab:
			onKeyTab();
			break;
		default:
			break;
		}

		return keyboardInput;
	}
	virtual sf::Mouse::Button onMouseButtonPressed(sf::Event* eventRef)
	{
		const sf::Mouse::Button mouseInput = eventRef->mouseButton.button;

		switch (mouseInput)
		{
		case sf::Mouse::Left:
			onMouseClickL(eventRef);
			break;
		case sf::Mouse::Right:
			onMouseClickR(eventRef);
			break;
		case sf::Mouse::Middle:
			onMouseClickM(eventRef);
			break;
		default:
			break;
		}
		return mouseInput;
	}
	virtual sf::Mouse::Button onMouseButtonReleased(sf::Event* eventRef)
	{
		const sf::Mouse::Button mouseInput = eventRef->mouseButton.button;

		switch (mouseInput)
		{
		case sf::Mouse::Left:
			onMouseReleaseL(eventRef);
			break;
		case sf::Mouse::Right:
			onMouseReleaseR(eventRef);
			break;
		case sf::Mouse::Middle:
			onMouseReleaseM(eventRef);
			break;
		default:
			break;
		}
		return mouseInput;
	}
	virtual float onMouseScrolled(sf::Event* eventRef)
	{
		if (eventRef->mouseWheelScroll.wheel != sf::Mouse::VerticalWheel) return 0.0f;
		return eventRef->mouseWheelScroll.delta;
	}


	virtual bool onMouseMoved(sf::Event*) { return false; };
	virtual bool onMouseClickL(sf::Event*) { return false; };
	virtual bool onMouseClickR(sf::Event*) { return false; };
	virtual bool onMouseClickM(sf::Event*) { return false; };
	virtual void onMouseDownL() {};
	virtual void onMouseDownR() {};
	virtual void onMouseDownM() {};
	virtual bool onMouseReleaseL(sf::Event*) { return false; };
	virtual bool onMouseReleaseR(sf::Event*) { return false; };
	virtual bool onMouseReleaseM(sf::Event*) { return false; };

	virtual bool onKeyTab() { return false; }
	virtual bool onKeyEscape() { return false; }

public:
	virtual bool onLostFocus() { return false; };
	virtual bool handleEvent(sf::Event* eventRef)
	{
		switch (eventRef->type)
		{
		case sf::Event::KeyPressed:
			//std::cout << "[Keyboard Event]" << std::endl;
			return onKeyPressed(eventRef);
		case sf::Event::MouseButtonPressed:
			//std::cout << "[Mouse Event]" << std::endl;
			return onMouseButtonPressed(eventRef);
		case sf::Event::MouseWheelScrolled:
			//std::cout << "[Mouse Wheel Event]" << std::endl;
			return onMouseScrolled(eventRef);
		case sf::Event::MouseButtonReleased:
			return onMouseButtonReleased(eventRef);
		case sf::Event::MouseMoved:
			//std::cout << "[Mouse Moved Event]" << std::endl;
			return onMouseMoved(eventRef);
		case sf::Event::LostFocus:
			//std::cout << "[Focus Lost Event]" << std::endl;
			return onLostFocus();
		default:
			break;
		}

		return false;
	}

	virtual void idleInputs()
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			onMouseDownL();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			onMouseDownR();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
			onMouseDownM();
	}
};