export module WidgetElements;

#include "Button.h"
#include "Timer.h"

class WidgetWindow : public sf::Drawable
{
private:
	std::vector<sf::Drawable*> shapes;
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (sf::Drawable* elem : shapes) target.draw(*elem, states);
	}
};