#include "PerkFamilyTree.h"
#include <iostream>

// Implementation of tree generation methods
PerkTree PerkFamily_Tree::getOffensiveTree()
{
    return {
        "off_root", "Offense Root", "Start of offensive tree.",
        { // Children
            { "off_crit", "Critical Hit", "Increase crit chance." },
            { "off_dot", "Bleed", "Deal damage over time.",
            { // 1 Child
                { "off_dot_upgrade", "Deep Bleed", "Increase DoT duration." }
            }}
        }
    };
}

PerkTree PerkFamily_Tree::getDefensiveTree()
{
    return {
        "def_root", "Defense Root", "Fortify your resilience.",
        { // Children
            { "def_armor", "Hardened Skin", "Reduce incoming damage by 5%." },
            { "def_heal", "Second Wind", "Recover 10% HP after surviving a wave." },
            { "def_barrier", "Energy Barrier", "Gain a shield that absorbs 15 damage every 20 seconds.",
            { // 1 Child
                { "def_barrier2", "Barrier Recharge", "Reduce shield cooldown by 5 seconds." }
            }}
        }
    };
}

PerkTree PerkFamily_Tree::getUtilityTree()
{
    return {
        "util_root", "Utility Root", "Master versatility and control.",
        { // Children
            { "util_speed", "Quick Feet", "Move 10% faster." },
            { "util_reload", "Fast Hands", "Reload speed increased by 20%." },
            { "util_scan", "Enemy Scanner", "Reveals enemy health bars and weaknesses.",
            { // 1 Child
                { "util_scan2", "Auto-Target", "Highlights weak spots for bonus damage." }
            }}
        }
    };
}

PerkTree PerkFamily_Tree::getSupportTree()
{
    return {
        "sup_root", "Support Root", "Empower allies and enhance the battlefield.",
        { // Children
            { "sup_heal_aura", "Healing Aura", "Nearby allies regenerate 2% HP per second." },
            { "sup_ammo", "Ammo Dispenser", "Allies gain extra ammo on wave start." },
            { "sup_revive", "Quick Revive", "Revive allies 50% faster.",
            { // 1 Child
                { "sup_revive2", "Lifeline", "Grants temporary invincibility on revive." }
            }}
        }
    };
}

Button* PerkFamily_Tree::createButtonFromPerkInfo(const PerkNodeInfo& perkInfo)
{
    // Create a new button for this perk node
    Button* button = new Button(this);

    // Set button properties based on PerkNodeInfo
    button->setText(perkInfo.name);
    button->setText(perkInfo.tag);
    button->setEnabled(perkInfo.bUnlocked);

    button->construct();

    return button;
}

void PerkFamily_Tree::clearButtons()
{
    // Delete all existing buttons
    for (Button* button : perkButtons)
    {
        delete button;
    }
    perkButtons.clear();
	shapes.clear(); // Clear shapes as well
}

void PerkFamily_Tree::buildTree(const PerkNodeInfo& rootNode)
{
    std::cout << "Building tree" << std::endl;

    // Clear any existing buttons
    clearButtons();

    //TODO: REWORK WHOLE LOGIC FROM THIS LINE ON

    // Create the root button
    Button* rootButton = createButtonFromPerkInfo(rootNode);
    perkButtons.push_back(rootButton);
	shapes.push_back(rootButton);

    // Stack to keep track of parent buttons and their children info
    std::vector<std::pair<Button*, const std::vector<PerkNodeInfo>*>> nodeStack;
    nodeStack.push_back({ rootButton, &rootNode.children });

    // Node positioning constants
    const float HORIZONTAL_SPACING = 150.0f;  // Space between siblings
    const float VERTICAL_SPACING = 120.0f;    // Space between parent and children

    // Process all nodes in the tree
    while (!nodeStack.empty())
    {
        auto [parentButton, childrenInfo] = nodeStack.back();
        nodeStack.pop_back();

        if (childrenInfo->empty())
            continue;

        // Calculate positions for children
        float totalWidth = (childrenInfo->size() - 1) * HORIZONTAL_SPACING;
        float startX = parentButton->getPosition().x - totalWidth / 2.0f;
        float childY = parentButton->getPosition().y + VERTICAL_SPACING;

        // Create and position all children
        for (size_t i = 0; i < childrenInfo->size(); i++)
        {
            const PerkNodeInfo& childInfo = (*childrenInfo)[i];

            // Create child button
            Button* childButton = createButtonFromPerkInfo(childInfo);

            // Position the child
            float childX = startX + i * HORIZONTAL_SPACING;
            childButton->setPosition(sf::Vector2f(childX, childY));

            // Add to buttons collection
            perkButtons.push_back(childButton);
            shapes.push_back(childButton);

            // Add to stack if it has children
            if (!childInfo.children.empty())
            {
                nodeStack.push_back({ childButton, &childInfo.children });
            }
        }
    }
}

PerkFamily_Tree::PerkFamily_Tree(InputWidget* parent)
    : InputWidget(parent)
{
}

PerkFamily_Tree::~PerkFamily_Tree()
{
    clearButtons();
}

void PerkFamily_Tree::construct()
{
}

void PerkFamily_Tree::construct(const sf::Vector2f& startPos, const PerkFamily& pf)
{
    setPosition(startPos);
    displayedFamily = pf;
    // Building the tree on construct, should not be called more than once for each family
    rootNode = getPerkTree(pf);
    buildTree(rootNode);
}

bool PerkFamily_Tree::isMouseOver(const bool& checkForClick)
{
    // Check each button
    for (Button* button : perkButtons)
    {
        if (button->isMouseOver(checkForClick))
            return true;
    }

    return false;
}