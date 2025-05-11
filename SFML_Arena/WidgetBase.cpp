#pragma once 

#include "WidgetBase.h"
#include "GameInstance.h"
#include "SoundManager.h"

// WidgetMenu Code --------------------------------------------------------------------------------

sf::Vector2f WidgetElement::getTickCorrection() const
{
	if (parent)
		return parent->getTickCorrection();

	return IDrawableShapes::viewCenter;
}

WidgetElement::WidgetElement(InputWidget* parentWidget)
	: parent(parentWidget)
{

}

void WidgetElement::construct()
{
	IWidgetAnimation::playAnim(EAnimation::OPEN_ANIM);
}

void WidgetElement::tick(const float& deltaTime)
{
	if (tickPosCorrection != getTickCorrection())
	{
		tickPosCorrection = getTickCorrection();
	}

	tick_pos(tickPosCorrection);
	tick_anim(deltaTime);
}

void WidgetElement::tick_pos(const sf::Vector2f& withPos)
{
	static constexpr bool bTickUpdate = true;

	addPosition(withPos, bTickUpdate);
}

bool WidgetElement::isAnimBlockingInput() const
{
	bool bBlocking = false;

	if (parent)
		bBlocking |= parent->isAnimBlockingInput();

	bBlocking |= IWidgetAnimation::isAnimBlockingInput();

	return bBlocking;
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
