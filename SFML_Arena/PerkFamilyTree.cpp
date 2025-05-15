#include "PerkFamilyTree.h"
#include <iostream>
#include <array>

unsigned int PerkFamily_Tree::perkID = 0;

void PerkFamily_Tree::delegateEvents()
{
    // ONLY CALL AFTER BUILDING THE TREE

    for (std::unique_ptr<PerkNode>& node : perkButtons)
    {
        node->onClick = [this, button = node.get()]()
            {
                PerkNodeInfo* info = button->getNodeInfo();
                std::vector<PerkNodeInfo> children = info->children;

                for (PerkNodeInfo& childInfo : children)
                {
                    //nodeToButtonMap[childInfo.id]->construct();
                }
            };
    }
}

// Implementation of tree generation methods
const PerkTree& PerkFamily_Tree::getOffensiveTree() const
{
    static const PerkTree offensiveTree = {
        ++perkID, "off_root", "Offense Root", "Start of offensive tree.",
        { // 2 Children
            { ++perkID, "off_crit", "Critical Hit", "Increase crit chance." },
            { ++perkID, "off_dot", "Bleed", "Deal damage over time.",
            { // 1 Child
                { ++perkID, "off_dot_upgrade", "Deep Bleed", "Increase DoT duration." }
            }}
        }
    };

    return offensiveTree;
}

const PerkTree& PerkFamily_Tree::getDefensiveTree() const
{
    static const PerkTree defensiveTree = {
        ++perkID, "def_root", "Defense Root", "Fortify your resilience.",
        { // 3 Children
            { ++perkID, "def_armor", "Hardened Skin", "Reduce incoming damage by 5%." },
            { ++perkID, "def_heal", "Second Wind", "Recover 10% HP after surviving a wave." },
            { ++perkID, "def_barrier", "Energy Barrier", "Gain a shield that absorbs 15 damage every 20 seconds.",
            { // 2 Children
                { ++perkID, "def_barrier2", "Barrier Recharge", "Reduce shield cooldown by 5 seconds." },
                { ++perkID, "def_barrier_alt", "Barrier Recharge", "Reduce shield cooldown by 5 seconds." }
            }}
        }
    };

    return defensiveTree;
}

const PerkTree& PerkFamily_Tree::getUtilityTree() const
{
    static const PerkTree utilityTree = {
        ++perkID, "util_root", "Utility Root", "Master versatility and control.",
        { // 3 Children
            { ++perkID, "util_speed", "Quick Feet", "Move 10% faster." },
            { ++perkID, "util_reload", "Fast Hands", "Reload speed increased by 20%." },
            { ++perkID, "util_scan", "Enemy Scanner", "Reveals enemy health bars and weaknesses.",
            { // 1 Child
                { ++perkID, "util_scan2", "Auto-Target", "Highlights weak spots for bonus damage." }
            }}
        }
    };

    return utilityTree;
}

const PerkTree& PerkFamily_Tree::getSupportTree() const
{
    static const PerkTree  supportTree = {
        ++perkID, "sup_root", "Support Root", "Empower allies and enhance the battlefield.",
        { // 3 Children
            { ++perkID, "sup_heal_aura", "Healing Aura", "Nearby allies regenerate 2% HP per second." },
            { ++perkID, "sup_ammo", "Ammo Dispenser", "Allies gain extra ammo on wave start." },
            { ++perkID, "sup_revive", "Quick Revive", "Revive allies 50% faster.",
            { // 1 Child
                { ++perkID, "sup_revive2", "Lifeline", "Grants temporary invincibility on revive." }
            }}
        }
    };

    return supportTree;
}

const PerkTree& PerkFamily_Tree::getPerkTree(const PerkFamily& family)
{
    static const std::unordered_map<PerkFamily, const PerkTree&> perkTrees = {
        { PerkFamily::Offensive, getOffensiveTree() },
        { PerkFamily::Defensive, getDefensiveTree() },
        { PerkFamily::Utility,   getUtilityTree() },
        { PerkFamily::Support,   getSupportTree() },
    };

    auto it = perkTrees.find(family);
    if (it != perkTrees.end())
        return it->second;

    // Getting an empty tree in case family was invalid
    static const PerkTree emptyTree = { 0, "Invalid", "Invalid", "No tree available." };
    return emptyTree;
}

std::unique_ptr<PerkNode> PerkFamily_Tree::createButtonFromPerkInfo(PerkNodeInfo* perkInfo)
{
    // Create a new button for this perk node
    std::unique_ptr<PerkNode> node = std::make_unique<PerkNode>(this, perkInfo);
    node->construct(); // Construct the button with its info

    return std::move(node);
}

void PerkFamily_Tree::clearNodes()
{
    shapes.clear(); // Clear shapes as well, as they should only contain nodes
    perkButtons.clear();

    // Clear pointer maps and vectors
    nodeToButtonMap.clear();
    nodeConnections.clear();
    childToParent.clear();
}

sf::Vector2u PerkFamily_Tree::buildTreeLayout(PerkNodeInfo& rootNode)
{
    static constexpr unsigned int MAX_DEPTH = 5; // Maximum depth of the tree, for recursion safety

    // Store subtree height (number of vertical slots needed)
    std::unordered_map<const PerkNodeInfo*, int> nodeSubtreeHeight; // Storing with nodeInfo as Pointers for unique access and avoidance of copies being made

    // STEP 1: MEASURE SUBTREE HEIGHTS
    std::function<int(const PerkNodeInfo*)> measureSubtree;
    measureSubtree = [&](const PerkNodeInfo* node) -> int // -> int because of recursion, is safer
        {
            if (node->children.empty())
            {
                nodeSubtreeHeight[node] = 1; // Leaf node has height of 1, no children branches
                return 1;
            }

            int total = 0; // Initialize total height for this node
            for (const PerkNodeInfo& child : node->children)
            {
                total += measureSubtree(&child);
            }

            nodeSubtreeHeight[node] = total; // Store the total height of this node

            return total;
        };

    // STEP 2: BUILD BUTTONS AND POSITION THEM
    int maxDepthReached = 0;
    int totalVerticalSlots = measureSubtree(&rootNode); // fills nodeSubtreeHeight + gets max vertical

    clearNodes(); // Clear previous buttons if necessary

    const sf::Vector2f tlCorner = -getSize() / 2.0f; // Top-left corner of the visual tree area

    float x_start = tlCorner.x + padding; // Start position for x-axis, left most point inside border
    x_start += sideMenuWidth + padding; // Offset for the side menu width
    float y_start = tlCorner.y; // Start position for y-axis, calculated from the top-left corner

    // Step 1: Precompute total tree height from the root for vertical centering
    int totalTreeHeight = nodeSubtreeHeight[&rootNode]; // Total number of rows

    float y_step = NODE_SIZE.y + NODE_DISTANCE.y;       // Vertical space per node
    float totalPixelHeight = totalTreeHeight * y_step;
    float y_center_offset = -totalPixelHeight / 2.0f;    // Center offset (shifts root upward)

    std::function<void(PerkNodeInfo*, int, float)> placeButtons;
    placeButtons = [&](PerkNodeInfo* node, int depth, float y_base)
        {
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
            button->setPosition(position);

            PerkNode* rawPtr = button.get();
            nodeToButtonMap[node->id] = rawPtr; // Associate node with its Button (raw pointer, be careful!!!)

            perkButtons.push_back(std::move(button)); // Move the button into storage

            // Recursively place child nodes below this one
            float childCursorY = y_base;
            for (PerkNodeInfo& child : node->children)
            {
                nodeConnections.emplace_back(node, &child); // Associate parent-child nodes

                // Recursively place child button first
                int childHeight = nodeSubtreeHeight[&child];
                placeButtons(&child, depth + 1, childCursorY);
                childCursorY += childHeight * y_step;

                // Drawing line from parent to child
                PerkNode* parentBtn = nodeToButtonMap[node->id];
                PerkNode* childBtn = nodeToButtonMap[child.id];

                childToParent[&child] = node; // Store parent-child relationship
            }
        };


    // STEP 3: CALL BUILDER
    placeButtons(&rootNode, 0, 0.0f);

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

    treeSize = buildTreeLayout(rootNode);

    delegateEvents();
}

void PerkFamily_Tree::construct(const PerkFamily& pf)
{
    displayedFamily = pf;
    // Building the tree on construct, should not be called more than once for each family
    rootNode = getPerkTree(pf);
    rootNode.bUnlocked = true; // Set root node as unlocked
}

bool PerkFamily_Tree::isMouseOver(const bool& checkForClick)
{
    // Check each button (tree node)
    for (std::unique_ptr<PerkNode>& button : perkButtons)
    {
        if (button->isMouseOver(checkForClick))
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
        PerkNode* parentBtn = nodeToButtonMap.at(parent->id);
        PerkNode* childBtn = nodeToButtonMap.at(child->id);

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
            sf::Color lineColor = (child->bUnlocked ? sf::Color::White : sf::Color(100, 0, 0, 255));

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
        target.draw(*node, states);
    }
}