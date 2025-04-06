#pragma once 

#include "WidgetBase.h"
#include "GameInstance.h"

// WidgetMenu Code --------------------------------------------------------------------------------

WidgetElement::WidgetElement(InputWidget* parentWidget)
	: parent(parentWidget)
{

}

// InputWidget ------------------------------------------------------------------------------------

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
		lockWeakPtr( gameInstance().getActiveWidget())->input_esc();
		break;
	default:
		break;
	}
	return eventRef->key.code;
}

bool InputWidget::input_esc()
{
	if (isChildActive()) 
		return getActiveChild()->input_esc();
	
	if (parent != nullptr)
	{
		parent->setWidgetIndex(0)->construct();
		return true;
	}

	return false;
}