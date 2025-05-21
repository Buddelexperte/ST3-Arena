#pragma once

#include "DrawableShape.h"
#include "GameInstance.h"

sf::RenderWindow* IDrawableShapes::window = nullptr;
sf::Vector2u IDrawableShapes::windowSize = { 0, 0 };
sf::Vector2f IDrawableShapes::windowCenter = { 0.0f, 0.0f };
sf::View * IDrawableShapes::view = nullptr;
sf::Vector2f IDrawableShapes::viewSize = { 0.0f, 0.0f };
sf::Vector2f IDrawableShapes::viewHalfSize = { 0.0f, 0.0f };
sf::Vector2f IDrawableShapes::viewTL = { 0.0f, 0.0f };
sf::Vector2f IDrawableShapes::viewCenter = { 0.0f, 0.0f };
sf::Vector2f IDrawableShapes::widgetOffset = { 0.0f, 0.0f };
sf::Vector2f IDrawableShapes::buttonSize = { 300.0f, 100.0f };
sf::Vector2f IDrawableShapes::unitNorm = { 1.0f, 1.0f };
sf::Vector2f IDrawableShapes::viewSizeNorm = { 1.0f, 1.0f };

void IDrawableShapes::initValues()
{
	// Everything sf::RenderWindow related
	window = gameInstance().getWindow();
	windowSize = window->getSize();
	windowCenter = sf::Vector2f{ windowSize.x / 2.0f, windowSize.y / 2.0f };
	// Everything sf::View related
	view = gameInstance().getView();

	updateValues();
}

void IDrawableShapes::updateValues()
{
	viewSize = view->getSize();
	viewHalfSize = viewSize / 2.0f;
	viewCenter = view->getCenter();
	viewTL = viewCenter - viewHalfSize;

	widgetOffset = gameInstance().getWidgetOffset();

	// Decide wether to use this for scalability, needs different device testing
	viewSizeNorm = sf::Vector2f( viewSize.x / 2560.0f, viewSize.y / 1440.0f);
	unitNorm = viewSizeNorm;
	unitNorm = sf::Vector2f(1.0f, 1.0f);
	buttonSize = buttonRatio * unitNorm;
}

void IDrawableShapes::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& elem : shapes)
	{
		target.draw(*elem, states);
	}
}