#pragma once

#include "Button.h"
#include "PerkInfo.h"

class PerkNode : public Button
{
private:
	static const inline sf::Vector2f NODE_SIZE = sf::Vector2f(120.0f, 120.0f); // Size of the node

	PerkNodeInfo* nodeInfo; // Information about this perk node
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

		sf::Color nodeColor = nodeInfo->bUnlocked ? sf::Color::White : sf::Color(100, 20, 20, 255); // Color based on unlocked status

		RawButton buttonConstructor = {
			sf::Vector2f(0.0f, 0.0f), NODE_SIZE, nodeColor, 16, nodeInfo->tag, sf::Color::Black, EAlignment::CENTER, EAlignment::CENTER
		};

		// Set button properties based on PerkNodeInfo
		bool bStartDisabled = !nodeInfo->bUnlocked;
		Button::construct(buttonConstructor, bStartDisabled); // Call the base class constructor
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