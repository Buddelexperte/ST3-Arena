#pragma once

#include "design.h"

class GI_Arena;

class IDrawableShapes : public sf::Drawable
{
protected:
	static sf::RenderWindow* window;
	static sf::Vector2u windowSize;
	static sf::Vector2f windowCenter;
	static sf::View* view;
	static sf::Vector2f viewSize;
	static sf::Vector2f viewHalfSize;
	static sf::Vector2f viewTL;
	static sf::Vector2f viewCenter;
	static sf::Vector2f widgetOffset;

	static sf::Vector2f unitNorm;
	static sf::Vector2f viewSizeNorm;

	static sf::Vector2f buttonSize;
	static sf::Texture* buttonTexture;

	std::vector<sf::Drawable*> shapes;

public:
	static void updateValues();
	static void initValues();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};