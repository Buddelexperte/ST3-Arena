#pragma once 

#include "WidgetBase.h"
#include "GameInstance.h"
#include "SoundManager.h"

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

bool InputWidget::onKeyEscape()
{
	if (isChildActive())
	{
		return getActiveChild()->onKeyEscape();
	}
	
	if (parent != nullptr)
	{
		parent->setWidgetIndex(0)->construct();
		return true;
	}

	return false;
}

bool InputWidget::onKeyTab()
{
	return false;
}
