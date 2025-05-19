#pragma once

#include "PerkFamilyTree.h"
#include <iostream>
#include <array>

#include "GameInstance.h"

unsigned int PerkFamily_Tree::perkID = 0;

PerkFamily_Tree::~PerkFamily_Tree()
{
    reset();
}

PerkNodeInfo* PerkFamily_Tree::createPerkNodeInfo(
        unsigned int id, 
        const std::string& tag, 
        const std::string& name, 
        const std::string& description)
{
    // Create a new PerkNodeInfo with shorter strings
    auto newNode = std::make_unique<PerkNodeInfo>();
    newNode->id = id;

    // Copy strings with size checks to prevent potential issues
    try {
        newNode->tag = tag;
        newNode->name = name;
        newNode->description = description;
    }
    catch (const std::length_error& e) {
        std::cerr << "String too long error in createPerkNodeInfo: " << e.what() << std::endl;
        // Handle error - maybe truncate strings or use defaults
        if (newNode->tag.empty()) newNode->tag = "default_tag";
        if (newNode->name.empty()) newNode->name = "Default Name";
        if (newNode->description.empty()) newNode->description = "Default description";
    }

    PerkNodeInfo* nodePtr = newNode.get();
    nodeInfos.push_back(std::move(newNode));
    return nodePtr;
}

void PerkFamily_Tree::markParentNodesPath(PerkNodeInfo* node, const bool& bMark = true)
{
    PerkNodeInfo* ptr = node;
    while (childToParent[ptr] != nullptr)
    {
        PerkNodeInfo* parent = childToParent[ptr];
        parent->isPathHovered = bMark;
        nodeToButtonMap[parent->id]->onUpdate();

        ptr = parent;
    }
}

void PerkFamily_Tree::unlockChildrenNodes(PerkNodeInfo* node)
{
    std::vector<PerkNodeInfo*>& children = node->children;
    for (PerkNodeInfo* childInfo : children)
    {
        childInfo->bUnlocked = true;
        nodeToButtonMap[childInfo->id]->onUpdate();
    }
}

void PerkFamily_Tree::blockSiblingNodes(PerkNodeInfo* node)
{
    // If he has parent, block siblings
    if (childToParent[node] == nullptr)
        return;

    PerkNodeInfo* parent = childToParent[node];
    std::vector<PerkNodeInfo*>& siblings = parent->children;
    for (PerkNodeInfo* childInfo : siblings)
    {
        if (childInfo == node) continue; // Skip self as 'sibling'

        childInfo->bUnlocked = false;
        nodeToButtonMap[childInfo->id]->onUpdate();
    }
}

void PerkFamily_Tree::delegateEvents()
{
    // ONLY CALL AFTER BUILDING THE TREE

    // Setting all delegates dynamically for each node
    for (std::unique_ptr<PerkNode>& node : perkButtons)
    {
        // TODO: Tooltip menu on the side of the border
        node->onHover = [this, button = node.get()]()
            {
                PerkNodeInfo* info = button->getNodeInfo();
                markParentNodesPath(info, true);
            };

        node->onUnhover = [this, button = node.get()]()
            {
                PerkNodeInfo* info = button->getNodeInfo();
                markParentNodesPath(info, false);
            };

        node->onClick = [this, button = node.get()]()
            {
                PerkNodeInfo* info = button->getNodeInfo();
                info->bSelected = true;
                button->onUpdate();

                // Block siblings from being clicked
                blockSiblingNodes(info);

                // Unlock children if selected
                unlockChildrenNodes(info);

                markParentNodesPath(info, false);

                // Add the clicked perk (only not already selected ones are clickable, so no worries)
                gameInstance().getInventory().addPerk_byTag(info->tag);
            };
    }
}

// Implementation of tree generation methods
PerkTree PerkFamily_Tree::getOffensiveTree()
{
    PerkNodeInfo* root = createPerkNodeInfo(++perkID, "off_root", "Offense Root", "Start of offensive tree.");

    PerkNodeInfo* critNode = createPerkNodeInfo(++perkID, "off_crit", "Critical Hit", "Increase crit chance.");
    PerkNodeInfo* dotNode = createPerkNodeInfo(++perkID, "off_dot", "Bleed", "Deal damage over time.");
    PerkNodeInfo* dotUpgradeNode = createPerkNodeInfo(++perkID, "off_dot_upgrade", "Deep Bleed", "Increase DoT duration.");

    // Add children to nodes
    dotNode->children.push_back(dotUpgradeNode);
    root->children.push_back(critNode);
    root->children.push_back(dotNode);

    return root;
}

PerkTree PerkFamily_Tree::getDefensiveTree()
{
    PerkNodeInfo* root = createPerkNodeInfo(++perkID, "def_root", "Defense Root", "Fortify your resilience.");

    return root;
}

PerkTree PerkFamily_Tree::getUtilityTree()
{
    PerkNodeInfo* root = createPerkNodeInfo(++perkID, "util_root", "Utility Root", "Master versatility and control.");

    PerkNodeInfo* speedNode = createPerkNodeInfo(++perkID, "util_speed", "Quick Feet", "Move 10% faster.");
    PerkNodeInfo* reloadNode = createPerkNodeInfo(++perkID, "util_reload", "Fast Hands", "Reload speed increased by 20%.");
    PerkNodeInfo* scanNode = createPerkNodeInfo(++perkID, "util_scan", "Enemy Scanner", "Reveals enemy health bars and weaknesses.");

    PerkNodeInfo* scanUpgradeNode = createPerkNodeInfo(++perkID, "util_scan2", "Auto-Target", "Highlights weak spots for bonus damage.");

    // Add children
    scanNode->children.push_back(scanUpgradeNode);

    root->children.push_back(speedNode);
    root->children.push_back(reloadNode);
    root->children.push_back(scanNode);

    return root;
}

PerkTree PerkFamily_Tree::getSupportTree()
{
    PerkNodeInfo* root = createPerkNodeInfo(++perkID, "sup_root", "Support Root", "Empower allies and enhance the battlefield.");

    PerkNodeInfo* healAuraNode = createPerkNodeInfo(++perkID, "sup_heal_aura", "Healing Aura", "Nearby allies regenerate 2% HP per second.");
    PerkNodeInfo* ammoNode = createPerkNodeInfo(++perkID, "sup_vampire", "Ammo Dispenser", "Allies gain extra ammo on wave start.");
    PerkNodeInfo* reviveNode = createPerkNodeInfo(++perkID, "sup_shield", "Quick Revive", "Revive allies 50% faster.");

    PerkNodeInfo* reviveUpgradeNode = createPerkNodeInfo(++perkID, "sup_double_life", "Lifeline", "Grants temporary invincibility on revive.");

    // Add children
    reviveNode->children.push_back(reviveUpgradeNode);

    root->children.push_back(healAuraNode);
    root->children.push_back(ammoNode);
    root->children.push_back(reviveNode);

    return root;
}

PerkTree PerkFamily_Tree::getPerkTree(const PerkFamily& family)
{
    switch (family)
    {
    case PerkFamily::Offensive:
        return getOffensiveTree();
    case PerkFamily::Defensive:
        return getDefensiveTree();
    case PerkFamily::Utility:
        return getUtilityTree();
    case PerkFamily::Support:
        return getSupportTree();
    default:
        // Create an empty tree for invalid family
        return createPerkNodeInfo(0, "Invalid", "Invalid", "No tree available.");
    }
}

std::unique_ptr<PerkNode> PerkFamily_Tree::createButtonFromPerkInfo(PerkNodeInfo* perkInfo)
{
    if (!perkInfo) {
        std::cerr << "Error: Null PerkNodeInfo in createButtonFromPerkInfo" << std::endl;
        return nullptr;
    }

    // Create a new button for this perk node
    std::unique_ptr<PerkNode> node = std::make_unique<PerkNode>(this, perkInfo);
    node->construct(); // Construct the button with its info

    return node;
}

sf::Vector2u PerkFamily_Tree::buildTreeLayout(PerkNodeInfo* rootNode)
{
    if (!rootNode)
    {
        std::cerr << "Error: Null root node in buildTreeLayout" << std::endl;
        return sf::Vector2u(0, 0);
    }

    static constexpr unsigned int MAX_DEPTH = 5; // Maximum depth of the tree, for recursion safety

    // Store subtree height (number of vertical slots needed)
    std::unordered_map<PerkNodeInfo*, int> nodeSubtreeHeight; // Now using pointers

    // STEP 1: MEASURE SUBTREE HEIGHTS
    std::function<int(PerkNodeInfo*)> measureSubtree;
    measureSubtree = [&](PerkNodeInfo* node) -> int
        {
            if (!node) return 0;

            if (node->children.empty())
            {
                nodeSubtreeHeight[node] = 1; // Leaf node has height of 1, no children branches
                return 1;
            }

            int total = 0; // Initialize total height for this node
            for (PerkNodeInfo* child : node->children)
            {
                total += measureSubtree(child);
            }

            nodeSubtreeHeight[node] = total; // Store the total height of this node

            return total;
        };

    // STEP 2: BUILD BUTTONS AND POSITION THEM
    int maxDepthReached = 0;
    int totalVerticalSlots = measureSubtree(rootNode); // fills nodeSubtreeHeight + gets max vertical

    const sf::Vector2f tlCorner = -getSize() / 2.0f; // Top-left corner of the visual tree area TODO: Make position dependant

    float x_start = tlCorner.x + padding; // Start position for x-axis, left most point inside border
    x_start += sideMenuWidth + padding; // Offset for the side menu width
    float y_start = tlCorner.y; // Start position for y-axis, calculated from the top-left corner

    // Step 1: Precompute total tree height from the root for vertical centering
    int totalTreeHeight = nodeSubtreeHeight[rootNode]; // Total number of rows

    float y_step = NODE_SIZE.y + NODE_DISTANCE.y;       // Vertical space per node
    float totalPixelHeight = totalTreeHeight * y_step;
    float y_center_offset = -totalPixelHeight / 2.0f;    // Center offset (shifts root upward)

    std::function<void(PerkNodeInfo*, int, float)> placeButtons;
    placeButtons = [&](PerkNodeInfo* node, int depth, float y_base)
        {
            if (!node) return;

            if (depth >= static_cast<int>(MAX_DEPTH)) // Prevent going deeper than max depth
                return;

            maxDepthReached = std::max(maxDepthReached, depth); // Track max tree depth reached

            int subtreeHeight = nodeSubtreeHeight[node]; // Total node count of subtree rooted here

            // Calculate node world position
            float x = x_start + depth * (NODE_SIZE.x + NODE_DISTANCE.x);
            float y = y_center_offset + y_base + (subtreeHeight * y_step) / 2.0f; // Vertically center within subtree

            // Create and place the node's button
            sf::Vector2f position = { x, y };
            std::unique_ptr<PerkNode> button = createButtonFromPerkInfo(node);

            if (!button) {
                std::cerr << "Error: Failed to create button from PerkNodeInfo" << std::endl;
                return;
            }

            button->setPosition(position);

            PerkNode* rawPtr = button.get();
            nodeToButtonMap[node->id] = rawPtr; // Associate node with its Button

            perkButtons.push_back(std::move(button)); // Move the button into storage

            // Recursively place child nodes below this one
            float childCursorY = y_base;
            for (PerkNodeInfo* child : node->children)
            {
                if (!child) {
                    std::cerr << "Warning: Null child node found" << std::endl;
                    continue;
                }

                nodeConnections.emplace_back(node, child); // Associate parent-child nodes

                // Recursively place child button first
                int childHeight = nodeSubtreeHeight[child];
                placeButtons(child, depth + 1, childCursorY);
                childCursorY += childHeight * y_step;

                // Drawing line from parent to child
                PerkNode* parentBtn = nodeToButtonMap[node->id];

                auto childBtnIt = nodeToButtonMap.find(child->id);
                if (childBtnIt != nodeToButtonMap.end())
                {
                    PerkNode* childBtn = childBtnIt->second;
                    childToParent[child] = node; // Store parent-child relationship
                }
            }
        };


    // STEP 3: CALL BUILDER
    placeButtons(rootNode, 0, 0.0f);

    // Final size: depth and max vertical branches
    sf::Vector2u treeSize(totalVerticalSlots, maxDepthReached + 1); // +1 for the root node
    return treeSize;
}

PerkFamily_Tree::PerkFamily_Tree(InputWidget* parent)
    : InputWidget(parent)
{

}

void PerkFamily_Tree::construct()
{
    // Only build if not already built
    if (!perkButtons.empty())
        return;

    // Stop if rootNode nullptr as it is needed for build
    if (!rootNode)
    {
        std::cerr << "Error: rootNode is null in construct()" << std::endl;
        return;
    }

    treeSize = buildTreeLayout(rootNode);
    delegateEvents();
}

void PerkFamily_Tree::construct(const PerkFamily& pf)
{
    // Only create if this tree was not already built
    if (rootNode != nullptr)
    {
        construct();
        return;
    }

    displayedFamily = pf;
    reset(); // Clear all to avoid partial initialization

    // Building the tree on construct, should not be called more than once for each family
    rootNode = getPerkTree(pf);

    if (rootNode == nullptr) {
        std::cerr << "Error: Failed to create perk tree for family: " << static_cast<int>(pf) << std::endl;
        return;
    }

    rootNode->bUnlocked = true; // Set root node as unlocked
}

void PerkFamily_Tree::reset()
{
    rootNode = nullptr;

    for (std::unique_ptr<PerkNode>& elem : perkButtons)
    {
        elem.release();
    }
    perkButtons.clear();
    shapes.clear();

    for (std::unique_ptr<PerkNodeInfo>& info : nodeInfos)
    {
        info.release();
    }
    nodeInfos.clear();

    nodeToButtonMap.clear();
    nodeConnections.clear();
    childToParent.clear();
}

void PerkFamily_Tree::tick(const float& deltaTime)
{
    InputWidget::tick(deltaTime);

    // Ticking all perk nodes
    for (std::unique_ptr<PerkNode>& node : perkButtons)
    {
        node->tick(deltaTime);
    }
}

bool PerkFamily_Tree::isMouseOver(const bool& checkForClick)
{
    // Check each button (tree node)
    for (std::unique_ptr<PerkNode>& button : perkButtons)
    {
        if (button && button->isMouseOver(checkForClick))
            return true;
    }

    return false;
}

void PerkFamily_Tree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Create a single vertex array for all connection quads
    sf::VertexArray allLines(sf::Quads);

    for (const auto& [parent, child] : nodeConnections)
    {
        if (!parent || !child) continue;

        auto parentIt = nodeToButtonMap.find(parent->id);
        auto childIt = nodeToButtonMap.find(child->id);

        if (parentIt == nodeToButtonMap.end() || childIt == nodeToButtonMap.end())
            continue;

        PerkNode* parentBtn = parentIt->second;
        PerkNode* childBtn = childIt->second;

        if (parentBtn && childBtn)
        {
            const sf::Vector2f tickCorr = getTickCorrection();

            RawButton parentData = parentBtn->getButtonData();
            RawButton childData = childBtn->getButtonData();

            sf::Vector2f lineOffsetFromButtonCenter((NODE_SIZE.x - lineWidth) / 2.0f, 0.0f);

            sf::Vector2f from = parentData.pos + lineOffsetFromButtonCenter + tickCorr;
            sf::Vector2f to = childData.pos - lineOffsetFromButtonCenter + tickCorr;

            // Calc direction from parent to child and length of connection
            sf::Vector2f direction = to - from;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            // Normalize direction and calculate perpendicular offset
            sf::Vector2f dirNormalized = direction / length;
            sf::Vector2f perpendicular = { -dirNormalized.y, dirNormalized.x };
            sf::Vector2f offset = perpendicular * (lineWidth / 2.0f);

            // Set color based on unlocked status
            sf::Color lineColor = childBtn->getActualColor();

            // Push 4 vertices per connection into the shared array
            allLines.append({ from + offset, lineColor });
            allLines.append({ from - offset, lineColor });
            allLines.append({ to - offset, lineColor });
            allLines.append({ to + offset, lineColor });
        }
    }

    // Now draw all connections in a single call
    target.draw(allLines, states);

    // Draw buttons
    for (const std::unique_ptr<PerkNode>& node : perkButtons)
    {
        if (node)
            target.draw(*node, states);
    }
}