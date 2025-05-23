#pragma once

#include "Button.h"
#include "PerkInfo.h"

class PerkNode : public Button
{
private:
	const sf::Color getHoverColor() const override;

	const sf::Color getBeforeHoverColor() const;

	static const inline sf::Vector2f NODE_SIZE = sf::Vector2f(120.0f, 120.0f); // Size of the node

	PerkNodeInfo* nodeInfo; // Information about this perk node
	const sf::Color getNodeColor(const PerkNodeInfo*) const;

	const bool getShouldDisable(const PerkNodeInfo*) const;

public:
	PerkNode(InputWidget* parent, PerkNodeInfo* info);

	void construct();

	void onUpdate();

	void setNodeInfo(PerkNodeInfo* info);
	PerkNodeInfo* getNodeInfo();

	void setColor(const sf::Color& newColor) override;

	static sf::Vector2f getNodeSize();
};