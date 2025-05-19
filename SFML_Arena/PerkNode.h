#pragma once

#include "Button.h"
#include "PerkInfo.h"

class PerkNode : public Button
{
protected:
	virtual const sf::Color getHoverColor() const override
	{
		return sf::Color::Yellow;
	}

private:
	static const inline sf::Vector2f NODE_SIZE = sf::Vector2f(120.0f, 120.0f); // Size of the node

	PerkNodeInfo* nodeInfo; // Information about this perk node
	const sf::Color getNodeColor(const PerkNodeInfo*)
	{
		sf::Color nodeColor = sf::Color(100, 20, 20, 255);

		if (nodeInfo->isPathHovered)
			return sf::Color::Yellow;

		if (nodeInfo->bUnlocked)
			nodeColor = sf::Color::White;

		if (nodeInfo->bSelected)
			nodeColor -= getHoverDelta();

		return nodeColor;

	}

	const bool getShouldDisable(const PerkNodeInfo*)
	{
		return !nodeInfo->bUnlocked || nodeInfo->bSelected;
	}

public:
	PerkNode(InputWidget* parent, PerkNodeInfo* info)
		: Button(parent), nodeInfo(info)
	{

	}

	void construct()
	{
		// Return if no node info is set
		if (!nodeInfo)
			return;

		sf::Vector2f nodePos = getButtonData().pos;
		sf::Color nodeColor = getNodeColor(nodeInfo);

		static constexpr unsigned int fontSize = 16;

		RawButton buttonConstructor = {
			nodePos, NODE_SIZE, nodeColor, fontSize, nodeInfo->tag, sf::Color::Black, EAlignment::CENTER, EAlignment::CENTER
		};

		// Set button properties based on PerkNodeInfo
		bool bStartDisabled = getShouldDisable(nodeInfo);
		Button::construct(buttonConstructor, bStartDisabled); // Call the base class constructor
	}

	void onUpdate()
	{
		// Return if no node info is set
		if (!nodeInfo)
			return;

		setColor(getNodeColor(nodeInfo));
		setEnabled(!getShouldDisable(nodeInfo));
	}

	void setNodeInfo(PerkNodeInfo* info)
	{
		nodeInfo = info;
	}

	PerkNodeInfo* getNodeInfo()
	{
		return nodeInfo;
	}

	static sf::Vector2f getNodeSize()
	{
		return NODE_SIZE;
	}
};