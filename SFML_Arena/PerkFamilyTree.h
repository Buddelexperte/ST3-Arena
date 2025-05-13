#pragma once
#include "WidgetElements.h"
#include "PerkInfo.h"

// Structure to hold perk node information
struct PerkNodeInfo
{
	std::string tag;
	std::string name;
	std::string description;
	std::vector<PerkNodeInfo> children;
	bool bUnlocked = true;
	bool bSelected = false;
};

using PerkTree = PerkNodeInfo; // One start root node, now called Tree

class PerkFamily_Tree : public InputWidget
{
private:
	PerkFamily displayedFamily = PerkFamily::None;
	PerkTree rootNode; // Root node of the tree
	std::vector<Button*> perkButtons; // Store buttons for the tree

	// Create different perk trees
	PerkTree getOffensiveTree();
	PerkTree getDefensiveTree();
	PerkTree getUtilityTree();
	PerkTree getSupportTree();

	// Get the appropriate perk tree based on family
	inline const PerkTree& getPerkTree(const PerkFamily& family)
	{
		static const std::unordered_map<PerkFamily, PerkTree> perkTrees = {
			{ PerkFamily::Offensive, getOffensiveTree() },
			{ PerkFamily::Defensive, getDefensiveTree() },
			{ PerkFamily::Utility,   getUtilityTree() },
			{ PerkFamily::Support,   getSupportTree() },
		};
		auto it = perkTrees.find(family);
		if (it != perkTrees.end())
			return it->second;
		static const PerkTree emptyTree = { "invalid", "Invalid", "No tree available." };
		return emptyTree;
	}

	// Helper methods for tree building
	void clearButtons();
	void buildTree(const PerkNodeInfo& rootNode);
	Button* createButtonFromPerkInfo(const PerkNodeInfo& perkInfo);

public:
	PerkFamily_Tree(InputWidget* parent);
	~PerkFamily_Tree();

	void construct() override;
	void construct(const sf::Vector2f& startPos, const PerkFamily& pf);
	void tick(const float& deltaTime) override
	{
		InputWidget::tick(deltaTime);
		for (Button* button : perkButtons)
		{
			button->tick(deltaTime);
		}
	}

	void addPosition(const sf::Vector2f& delta, const bool& bTickBased = true) override
	{
		WidgetElement::addPosition(delta, bTickBased);
		// Update positions of all buttons
		for (Button* button : perkButtons)
		{
			button->addPosition(delta, bTickBased);
		}
	}

	bool isMouseOver(const bool& checkForClick = false) override;
};