#include "W_LevelUp.h"

W_LevelUp::W_LevelUp(InputWidget* parent)
	: InputWidget(parent)
{

}

void W_LevelUp::construct()
{

}

void W_LevelUp::tick(const float& deltaTime)
{

}

InputWidget* W_LevelUp::setWidgetIndex(const int& toIndex)
{
	shapes = { };

	switch ((widgetIndex = toIndex))
	{
	case 0: // SELF
		break;
	default:
		break;
	}

	return getWidgetAtIndex(widgetIndex);
}

InputWidget* W_LevelUp::getWidgetAtIndex(const int& atIndex)
{
	switch (atIndex)
	{
	case 0: // SELF
		return this;
		break;
	default:
		break;
	}

	return nullptr;
}