#pragma once 

#include "WidgetBase.h"
#include "GameInstance.h"
#include "SoundManager.h"

// WidgetMenu Code --------------------------------------------------------------------------------

sf::Vector2f WidgetElement::getTickCorrection() const
{
	if (parent != nullptr)
	{
		return parent->getTickCorrection();
	}

	return IDrawableShapes::viewCenter;
}

WidgetElement::WidgetElement(InputWidget* parentWidget)
	: parent(parentWidget)
{

}

bool WidgetElement::isAnimBlockingInput() const
{
	if (parent)
		return parent->isAnimBlockingInput();

	return IWidgetAnimation::isAnimBlockingInput();
}

// InputWidget ------------------------------------------------------------------------------------

bool InputWidget::onMouseMoved(sf::Event* eventRef)
{
	return isMouseOver(false);
}

bool InputWidget::onMouseClickL(sf::Event* eventRef)
{
	return isMouseOver(true);
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
