#pragma once 

#include "WidgetBase.h"
#include "GameInstance.h"

// WidgetMenu Code --------------------------------------------------------------------------------

WidgetElement::WidgetElement(InputWidget* parentWidget)
	: parent(parentWidget), gameInstance(&GI_Arena::getInstance()), window(gameInstance->getWindow())
{
	ORIGIN = window->getView().getCenter();
	windowUpdate();
}

void WidgetElement::windowUpdate()
{
	// Everything sf::RenderWindow related
	window = gameInstance->getWindow();
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
	// Everything sf::View related
	view = gameInstance->getView();
	viewSize = view->getSize();
	viewHalfSize = viewSize / 2.0f;
	viewCenter = view->getCenter();
}

void WidgetElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& elem : shapes) target.draw(*elem, states);
}


// InputWidget ------------------------------------------------------------------------------------

bool InputWidget::handleEvent(sf::Event* eventRef)
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
	default:
		break;
	}

	return false;
}

void InputWidget::idleInputs()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		onMouseDownL();

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		onMouseDownR();

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		onMouseDownM();
}

InputWidget* InputWidget::setWidgetIndex(const int& toIndex)
{
	widgetIndex = toIndex;
	switch (widgetIndex)
	{
	case 0:
		// Implement default shapes in child classes
		break;
	default:
		break;
	}
	return getWidgetAtIndex(toIndex);
}

sf::Keyboard::Key InputWidget::onKeyPressed(sf::Event* eventRef)
{
	switch (eventRef->key.code)
	{
	case sf::Keyboard::Escape:
		lockWeakPtr( gameInstance->getActiveWidget() )->input_esc();
		break;
	default:
		break;
	}
	return eventRef->key.code;
}

sf::Mouse::Button InputWidget::onMouseButtonPressed(sf::Event* eventRef)
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

sf::Mouse::Button InputWidget::onMouseButtonReleased(sf::Event* eventRef)
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

float InputWidget::onMouseScrolled(sf::Event* eventRef)
{
	if (event->mouseWheelScroll.wheel != sf::Mouse::VerticalWheel) return 0.0f;
	return eventRef->mouseWheelScroll.delta;
}


