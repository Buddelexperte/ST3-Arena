#pragma once

#include "Button.h"
#include "PerkInfo.h"

class PerkNode : public Button
{
private:
	const sf::Color getHoverColor() const override
	{
		return sf::Color::Yellow;
	}

	const sf::Color getBeforeHoverColor() const
	{
		return getNodeColor(nodeInfo);
	}

	static const inline sf::Vector2f NODE_SIZE = sf::Vector2f(120.0f, 120.0f); // Size of the node

	PerkNodeInfo* nodeInfo; // Information about this perk node
	const sf::Color getNodeColor(const PerkNodeInfo*) const
	{
		static const sf::Color blockedColor = sf::Color(100, 20, 20, 255);

		sf::Color nodeColor = blockedColor;
		
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

		sf::Color nodeColor = getNodeColor(nodeInfo);
		setColor(nodeColor);

		bool enableNode = !getShouldDisable(nodeInfo);
		setEnabled(enableNode);

		if (nodeInfo->isPathHovered)
		{
			stopAnim(EAnimation::ON_UNHOVER);
			playAnim(EAnimation::ON_HOVER);
		}
		else
		{
			stopAnim(EAnimation::ON_HOVER);
			playAnim(EAnimation::ON_UNHOVER);
		}
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