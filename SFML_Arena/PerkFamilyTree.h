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
	std::vector<std::unique_ptr<Button>> perkButtons; // Store buttons for the tree

	// Create different perk trees
	const PerkTree& getOffensiveTree() const;
	const PerkTree& getDefensiveTree() const;
	const PerkTree& getUtilityTree() const;
	const PerkTree& getSupportTree() const;

	// Get the appropriate perk tree based on family
	const PerkTree& getPerkTree(const PerkFamily& family);

	// Helper methods for tree building
	sf::Vector2u countTreeSize(const PerkNodeInfo& rootNode);
	void clearNodes();
	void buildTree(const PerkNodeInfo& rootNode);
	std::unique_ptr<Button> createButtonFromPerkInfo(const PerkNodeInfo&);

public:
	PerkFamily_Tree(InputWidget* parent);
	~PerkFamily_Tree();

	void construct() override;
	void construct(const sf::Vector2f& startPos, const PerkFamily& pf);
	void tick(const float& deltaTime) override
	{
		InputWidget::tick(deltaTime);

		// Ticking all perk nodes
		for (std::unique_ptr<Button>& button : perkButtons)
		{
			button->tick(deltaTime);
		}
	}

	void addPosition(const sf::Vector2f& delta, const bool& bTickBased = true) override
	{
		WidgetElement::addPosition(delta, bTickBased);

		// Update positions of all buttons
		for (std::unique_ptr<Button>& button : perkButtons)
		{
			button->addPosition(delta, bTickBased);
		}
	}

	bool isMouseOver(const bool& checkForClick = false) override;
};