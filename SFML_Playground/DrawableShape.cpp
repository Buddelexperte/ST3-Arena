#pragma once

#include "DrawableShape.h"
#include "GameInstance.h"

sf::RenderWindow* IDrawableShapes::window = nullptr;
sf::Vector2u IDrawableShapes::windowSize = { 0, 0 };
sf::Vector2f IDrawableShapes::windowCenter = { 0.0f, 0.0f };
sf::View * IDrawableShapes::view = nullptr;
sf::Vector2f IDrawableShapes::viewSize = { 0.0f, 0.0f };
sf::Vector2f IDrawableShapes::viewHalfSize = { 0.0f, 0.0f };
sf::Vector2f IDrawableShapes::viewCenter = { 0.0f, 0.0f };

void IDrawableShapes::updateValues()
{
	// Everything sf::RenderWindow related
	window = gameInstance().getWindow();
	windowSize = window->getSize();
	windowCenter = { windowSize.x / 2.0f, windowSize.y / 2.0f };
	// Everything sf::View related
	view = gameInstance().getView();
	viewSize = view->getSize();
	viewHalfSize = viewSize / 2.0f;
	viewCenter = view->getCenter();
}

void IDrawableShapes::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& elem : shapes) target.draw(*elem, states);
}