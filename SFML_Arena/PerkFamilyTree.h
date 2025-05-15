#pragma once
#include "WidgetElements.h"
#include "PerkInfo.h"
#include "PerkNode.h"

using PerkTree = PerkNodeInfo*; // Changed to pointer

class PerkFamily_Tree : public InputWidget
{
private:
    static unsigned int perkID;
    void delegateEvents() override;
    static constexpr float borderPadding = padding * 5.0f; // Padding for the border
    static const inline sf::Vector2f NODE_SIZE = PerkNode::getNodeSize(); // Size of the node
    static inline const sf::Vector2f NODE_DISTANCE = sf::Vector2f(200.0f, 50.0f); // Distance for each node
    static constexpr float sideMenuWidth = 500.0f; // Width of the side menu
    static constexpr float lineWidth = 5.0f; // Width of the line in pixels

    PerkFamily displayedFamily = PerkFamily::None; // Family this tree displays
    PerkTree rootNode = nullptr; // Root node of the tree (pointer)
    sf::Vector2u treeSize; // Size of the tree (width, depth)

    std::vector<std::unique_ptr<PerkNode>> perkButtons; // Store buttons for the tree

    // For line connections
    std::unordered_map<unsigned int, PerkNode*> nodeToButtonMap;
    std::vector<std::pair<PerkNodeInfo*, PerkNodeInfo*>> nodeConnections; // Changed to pointers
    std::unordered_map<PerkNodeInfo*, PerkNodeInfo*> childToParent; // Changed to pointers

    // Storage for all created PerkNodeInfo objects to manage memory
    std::vector<std::unique_ptr<PerkNodeInfo>> allPerkNodes;

    // Create different perk trees
    PerkTree getOffensiveTree();
    PerkTree getDefensiveTree();
    PerkTree getUtilityTree();
    PerkTree getSupportTree();

    // Get the appropriate perk tree based on family
    PerkTree getPerkTree(const PerkFamily& family);

    // Helper methods for tree building
    sf::Vector2u buildTreeLayout(PerkNodeInfo* rootNode);
    void clearNodes();
    std::unique_ptr<PerkNode> createButtonFromPerkInfo(PerkNodeInfo* perkInfo);

    // Helper for creating PerkNodeInfo objects and managing memory
    PerkNodeInfo* createPerkNodeInfo(unsigned int id, const std::string& tag,
        const std::string& name, const std::string& description);

public:
    PerkFamily_Tree(InputWidget* parent);
    ~PerkFamily_Tree(); // Added destructor to clean up memory

    void construct() override;
    void construct(const PerkFamily& pf);

    void tick(const float& deltaTime) override
    {
        InputWidget::tick(deltaTime);

        // Ticking all perk nodes
        for (std::unique_ptr<PerkNode>& node : perkButtons)
        {
            node->tick(deltaTime);
        }
    }

    bool isMouseOver(const bool& checkForClick = false) override;

    // Override for specific line drawing
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};