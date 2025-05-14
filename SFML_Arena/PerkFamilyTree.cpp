#include "PerkFamilyTree.h"
#include <iostream>
#include <array>

// Implementation of tree generation methods
const PerkTree& PerkFamily_Tree::getOffensiveTree() const
{
    static const PerkTree offensiveTree = {
        "off_root", "Offense Root", "Start of offensive tree.",
        { // 2 Children
            { "off_crit", "Critical Hit", "Increase crit chance." },
            { "off_dot", "Bleed", "Deal damage over time.",
            { // 1 Child
                { "off_dot_upgrade", "Deep Bleed", "Increase DoT duration." }
            }}
        }
    };

    return offensiveTree;
}

const PerkTree& PerkFamily_Tree::getDefensiveTree() const
{
    static const PerkTree defensiveTree = {
        "def_root", "Defense Root", "Fortify your resilience.",
        { // 3 Children
            { "def_armor", "Hardened Skin", "Reduce incoming damage by 5%." },
            { "def_heal", "Second Wind", "Recover 10% HP after surviving a wave." },
            { "def_barrier", "Energy Barrier", "Gain a shield that absorbs 15 damage every 20 seconds.",
            { // 1 Child
                { "def_barrier2", "Barrier Recharge", "Reduce shield cooldown by 5 seconds." }
            }}
        }
    };

    return defensiveTree;
}

const PerkTree& PerkFamily_Tree::getUtilityTree() const
{
    static const PerkTree utilityTree = {
        "util_root", "Utility Root", "Master versatility and control.",
        { // 3 Children
            { "util_speed", "Quick Feet", "Move 10% faster." },
            { "util_reload", "Fast Hands", "Reload speed increased by 20%." },
            { "util_scan", "Enemy Scanner", "Reveals enemy health bars and weaknesses.",
            { // 1 Child
                { "util_scan2", "Auto-Target", "Highlights weak spots for bonus damage." }
            }}
        }
    };

    return utilityTree;
}

const PerkTree& PerkFamily_Tree::getSupportTree() const
{
    static const PerkTree  supportTree = {
        "sup_root", "Support Root", "Empower allies and enhance the battlefield.",
        { // 3 Children
            { "sup_heal_aura", "Healing Aura", "Nearby allies regenerate 2% HP per second." },
            { "sup_ammo", "Ammo Dispenser", "Allies gain extra ammo on wave start." },
            { "sup_revive", "Quick Revive", "Revive allies 50% faster.",
            { // 1 Child
                { "sup_revive2", "Lifeline", "Grants temporary invincibility on revive." }
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
    static const PerkTree emptyTree = { "Invalid", "Invalid", "No tree available." };
    return emptyTree;
}

std::unique_ptr<Button> PerkFamily_Tree::createButtonFromPerkInfo(const PerkNodeInfo& perkInfo)
{
    // Create a new button for this perk node
    std::unique_ptr<Button> button = std::make_unique<Button>(this);

    // Set button properties based on PerkNodeInfo
    button->setText(perkInfo.name);
    button->setText(perkInfo.tag);
    button->setEnabled(perkInfo.bUnlocked);

    button->construct();

    return std::move(button);
}

void PerkFamily_Tree::clearNodes()
{
	shapes.clear(); // Clear shapes as well, as they should only contain nodes
    perkButtons.clear();
}

sf::Vector2u PerkFamily_Tree::countTreeSize(const PerkNodeInfo& rootNode)
{
    static constexpr unsigned int MAX_DEPTH = 10;

    // Array tracking the width per depth (capped at MAX_DEPTH)
    std::array<int, MAX_DEPTH> widthPerLevel = {};
    int maxDepthReached = 0; // Counter outside of function to keep track of max value

    // Recursive function for PerkInfoNode tree measurement
    std::function<void(const PerkNodeInfo&, int)> traverse;
    traverse = [&](const PerkNodeInfo& node, int depthReached)
        {
            if (depthReached >= MAX_DEPTH)
                return;

            // Track width (number of nodes per depth level)
            widthPerLevel[depthReached]++;

            // Track max depth reached by const parameter
            if (depthReached > maxDepthReached)
                maxDepthReached = depthReached;

            // Traverse children, recursively
            for (const PerkNodeInfo& child : node.children)
            {
                traverse(child, depthReached + 1);
            }
        };

    // Start at the rootNode, depth = 0
    traverse(rootNode, 0);

    // Find the maximum width from all levels
    int maxWidth = *std::max_element(widthPerLevel.begin(), widthPerLevel.end());
    maxDepthReached += 1; // +1 because root is at level 0

    sf::Vector2u treeSize(maxWidth, maxDepthReached);

    return treeSize;
}


void PerkFamily_Tree::buildTree(const PerkNodeInfo& rootNode)
{
    // Clear any existing buttons
    clearNodes();

    // Create the root button
    std::unique_ptr<Button> rootButton = createButtonFromPerkInfo(rootNode);
    perkButtons.push_back(std::move(rootButton));

    //TODO: REWORK WHOLE LOGIC FROM THIS LINE ON

    for (std::unique_ptr<Button>& button : perkButtons)
    {
        shapes.push_back(button.get());
    }

    sf::Vector2u treeSize = countTreeSize(rootNode);
}

PerkFamily_Tree::PerkFamily_Tree(InputWidget* parent)
    : InputWidget(parent)
{
}

PerkFamily_Tree::~PerkFamily_Tree()
{
    clearNodes();
}

void PerkFamily_Tree::construct()
{

}

void PerkFamily_Tree::construct(const sf::Vector2f& borderSize, const PerkFamily& pf)
{
    setSize(borderSize);
    displayedFamily = pf;
    // Building the tree on construct, should not be called more than once for each family
    rootNode = getPerkTree(pf);
    buildTree(rootNode);
}

bool PerkFamily_Tree::isMouseOver(const bool& checkForClick)
{
    // Check each button (tree node)
    for (std::unique_ptr<Button>& button : perkButtons)
    {
        if (button->isMouseOver(checkForClick))
            return true;
    }

    return false;
}