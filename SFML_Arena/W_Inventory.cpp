#include "W_Inventory.h"

W_Inventory::W_Inventory(InputWidget* parent)
	: InputWidget(parent)
{

}

void W_Inventory::construct()
{
	setWidgetIndex(0);
}

void W_Inventory::tick(const float& deltaTime)
{

}

InputWidget* W_Inventory::setWidgetIndex(const int& toIndex)
{

	switch (widgetIndex = toIndex)
	{
	case 0: // SELF
		shapes = { };
		break;
	default:
		shapes = { };
		break;
	}

	return getWidgetAtIndex(widgetIndex);
}

InputWidget* W_Inventory::getWidgetAtIndex(const int& atIndex)
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
