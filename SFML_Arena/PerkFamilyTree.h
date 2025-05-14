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
	bool bUnlocked = false;
	bool bSelected = false;
};

using PerkTree = PerkNodeInfo; // One start root node, now called Tree

class PerkFamily_Tree : public InputWidget
{
private:
	static constexpr float borderPadding = padding * 5.0f; // Padding for the border
	static inline const sf::Vector2f NODE_SIZE = sf::Vector2f(120.0f, 120.0f); // Width of each node
	static inline const sf::Vector2f NODE_DISTANCE = sf::Vector2f(200.0f, 50.0f); // Distance for each node
	static constexpr float sideMenuWidth = 500.0f; // Width of the side menu

	static constexpr float lineWidth = 5.0f; // Width of the line in pixels

	PerkFamily displayedFamily = PerkFamily::None; // Family this tree displays
	PerkTree rootNode; // Root node of the tree

	sf::Vector2u treeSize; // Size of the tree (width, depth)
	std::vector<std::unique_ptr<Button>> perkButtons; // Store buttons for the tree
	// TODO: Implement callback from button to PerkNodeInfo reference/pointer

	// For line connections
	std::unordered_map<const PerkNodeInfo*, Button*> nodeToButtonMap;
	std::vector<std::pair<const PerkNodeInfo*, const PerkNodeInfo*>> nodeConnections;

	// Create different perk trees
	const PerkTree& getOffensiveTree() const;
	const PerkTree& getDefensiveTree() const;
	const PerkTree& getUtilityTree() const;
	const PerkTree& getSupportTree() const;

	// Get the appropriate perk tree based on family
	const PerkTree& getPerkTree(const PerkFamily& family);

	// Helper methods for tree building
	sf::Vector2u buildTreeLayout(const PerkNodeInfo& rootNode);
	void clearNodes();
	void buildTree(const PerkNodeInfo& rootNode);
	std::unique_ptr<Button> createButtonFromPerkInfo(const PerkNodeInfo&);

public:
	PerkFamily_Tree(InputWidget* parent);

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

	bool isMouseOver(const bool& checkForClick = false) override;


	// Override for specific line drawing
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};