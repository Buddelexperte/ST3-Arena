#pragma once

#include "PerkNode.h"
#include "TextureManager.h"

const sf::Color PerkNode::getHoverColor() const
{
	return sf::Color::Yellow;
}

const sf::Color PerkNode::getBeforeHoverColor() const
{
	return getNodeColor(nodeInfo);
}

const sf::Color PerkNode::getNodeColor(const PerkNodeInfo*) const
{
	static const sf::Color blockedColor = sf::Color(100, 20, 20, 255);

	sf::Color nodeColor = blockedColor;

	if (nodeInfo->bUnlocked)
		nodeColor = sf::Color::White;

	if (nodeInfo->bSelected)
		nodeColor -= getHoverDelta();

	return nodeColor;

}

const bool PerkNode::getShouldDisable(const PerkNodeInfo*) const
{
	return !nodeInfo->bUnlocked || nodeInfo->bSelected;
}

PerkNode::PerkNode(InputWidget* parent, PerkNodeInfo* info)
	: Button(parent), nodeInfo(info)
{
	TextureManager& tm = TextureManager::getInstance();
	setTexture(tm.getTexturePtr(ETexture::BUTTON_SQR));
}

void PerkNode::construct()
{
	// Return if no node info is set
	if (!nodeInfo)
		return;

	sf::Vector2f nodePos = getButtonData().pos;
	sf::Color nodeColor = getNodeColor(nodeInfo);
	sf::Color textColor = lerp(normalTextColor, nodeColor, 0.5f);

	static constexpr unsigned int fontSize = 16;

	RawButton buttonConstructor = {
		nodePos, NODE_SIZE, nodeColor, fontSize, nodeInfo->tag, textColor, EAlignment::CENTER, EAlignment::CENTER
	};

	// Set button properties based on PerkNodeInfo
	bool bStartDisabled = getShouldDisable(nodeInfo);
	Button::construct(buttonConstructor, bStartDisabled); // Call the base class constructor
}

void PerkNode::onUpdate()
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

void PerkNode::setNodeInfo(PerkNodeInfo* info)
{
	nodeInfo = info;
}

PerkNodeInfo* PerkNode::getNodeInfo()
{
	return nodeInfo;
}

void PerkNode::setColor(const sf::Color& newColor)
{
	Button::setColor(newColor);
	setTextColor(lerp(normalTextColor, newColor, 0.5f));
}

sf::Vector2f PerkNode::getNodeSize()
{
	return NODE_SIZE;
}
