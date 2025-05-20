#pragma once

#include "ItemBaseClasses.h"
#include "Inventory.h"
#include "AllPerks.h"

Weapon::Weapon(const ItemInfo& info, const float& damage, std::unique_ptr<ProjectileSpawner> ps, std::unique_ptr<ValueBar> cd)
    :
    Item(info),
    damage(damage),
    projSpawner(std::move(ps)),
    cooldown(std::move(cd))
{
    cooldown->setValue(cooldown->getMaxValue() + 0.5f);
    // Weapons are considered "ready" by default
    bReady = true;
}

UseResult Weapon::activate(const ItemUse& use)
{
    switch (use)
    {
    case ItemUse::NO_USE:
        break;
    case ItemUse::ATTACK:
        {
            if (cooldown->isNotEmpty())
            {
                return UseResult::FAILURE_COOLDOWN;
            }
            // Add additional firing logic (ammo check, cooldowns, etc.) here.
            float cooldownDeficit = owningInventory->getCooldownSubtractor();
            cooldown->fill_to_max(cooldownDeficit);
        }
        return UseResult::SUCCESS;
        break;
    case ItemUse::LOAD_UP:
        break;
    case ItemUse::CANCEL_LOAD:
        break;
    case ItemUse::PASSIVE_TRIGGER:
        break;
    case ItemUse::INVALID_USE: default:
        break;
    }

    return UseResult::INVALID_USE;
}

void Weapon::tick(const float& deltaTime)
{
    float cooldownDelta = -deltaTime * owningInventory->getCooldownMultiplier();
    cooldown->addValue(cooldownDelta);

	activate(ItemUse::NO_USE);
}


// Weapon Factory
#include "AllWeapons.h"

const std::vector<StringWeapon> weaponMappings = {
    {"PISTOL", PISTOL},
    {"RIFLE", RIFLE},
    {"SHOTGUN", SHOTGUN},
    {"BURST RIFLE", BURST_RIFLE},
    {"RIFLE BURST", BURST_RIFLE},
    {"RIFLE_BURST", BURST_RIFLE}
};

std::string normalizeWeaponName(const std::string& name)
{
    std::string normalized;
    bool lastWasSpace = true; // To collapse multiple spaces

    for (char ch : name) {
        // Convert to uppercase and replace underscores with spaces
        if (ch == '_') ch = ' ';
        if (std::isspace(ch))
        {
            if (!lastWasSpace)
            {
                normalized.push_back(' '); // Only add one space
                lastWasSpace = true;
            }
        }
        else {
            normalized.push_back(std::toupper(ch));
            lastWasSpace = false;
        }
    }

    // Trim leading and trailing spaces
    if (!normalized.empty() && normalized.front() == ' ') {
        normalized.erase(normalized.begin());
    }
    if (!normalized.empty() && normalized.back() == ' ') {
        normalized.pop_back();
    }

    return normalized;
}

WeaponType getWeaponTypeFromText(const std::string& buttonText)
{
    std::string normalized = normalizeWeaponName(buttonText);
    // For simplicity, perform a linear search; for many entries, consider using a hash map.
    for (const auto& mapping : weaponMappings)
    {
        if (normalized == mapping.name)
        {
            return mapping.type;
        }
    }
    return UNKNOWN_WEAPON;
}

// Factory function for weapons from their weapon anme (string)
std::unique_ptr<Weapon> makeWeapon(const std::string& selectWeaponText)
{
    const WeaponType type = getWeaponTypeFromText(selectWeaponText);

    switch (type) {
    case PISTOL:
        return std::make_unique<Pistol>();
    case RIFLE:
        return std::make_unique<Rifle>();
    case SHOTGUN:
        return std::make_unique<Shotgun>();
    case BURST_RIFLE:
        return std::make_unique<Rifle_Burst>();
    default:
        break;
    }

    return nullptr;
}

std::unique_ptr<Perk> makePerk(const std::string& tag)
{
    static const std::unordered_map<std::string, std::function<std::unique_ptr<Perk>()>> perkFactory = {
        // Offensive
        {"off_root", [] { return std::make_unique<POff_Root>(); }},
        {"off_crit", [] { return std::make_unique<POff_Crit>(); }},
        {"off_dot", [] { return std::make_unique<POff_DOT>(); }},
        {"off_dot_upgrade", [] { return std::make_unique<POff_DOT_Upgrade>(); }},
        // Defense
        {"def_root", [] { return std::make_unique<PDef_Root>(); }},
        {"def_parry", [] { return std::make_unique<PDef_Parry>(); }},
        {"def_shield", [] { return std::make_unique<PDef_Shield>(); }},
        // Utility
        {"util_root", [] { return std::make_unique<PUtil_Root>(); }},
        {"util_speed", [] { return std::make_unique<PUtil_Speed>(); }},
        {"util_reload", [] { return std::make_unique<PUtil_Reload>(); }},
        {"util_scan", [] { return std::make_unique<PUtil_Scan>(); }},
        {"util_scan2", [] { return std::make_unique<PUtil_Scan2>(); }},
        // Support
        {"sup_root", [] { return std::make_unique<PSup_Root>(); }},
        {"sup_heal_aura", [] { return std::make_unique<PSup_HealAura>(); }},
        {"sup_vampire", [] { return std::make_unique<PSup_Vampire>(); }},
        {"sup_double_life", [] { return std::make_unique<PSup_DoubleLife>(); }}
    };

    auto it = perkFactory.find(tag);
    return (it != perkFactory.end()) ? it->second() : nullptr;
}

Perk::Perk(const ItemInfo& itemInfo, const std::unordered_set<PerkTrigger>& triggers)
    : Item(itemInfo), triggers(triggers)
{
    // Perks are always active/passive once acquired.
    bReady = true;
}

UseResult Perk::activate(const ItemUse& use)
{
    switch (use)
    {
    case ItemUse::ON_EQUIP:
        onEquip();
        return UseResult::SUCCESS;
    default:
        break;
    }

    return UseResult::FAILURE;
}

bool Perk::hasTrigger(const PerkTrigger& trigger)
{
    return triggers.contains(trigger);
}

void Perk::tryTrigger(PerkTriggerInfo& triggerInfo)
{
    const PerkTrigger trigger = triggerInfo.trigger;
    if (hasTrigger(trigger))
    {
        switch (trigger)
        {
        case PerkTrigger::OnWaveStart:
            onWaveStart(triggerInfo);
            break;

        case PerkTrigger::OnWaveEnd:
            onWaveEnd(triggerInfo);
            break;

        case PerkTrigger::OnEnemyContact:
            onEnemyContact(triggerInfo);
            break;

        case PerkTrigger::OnPlayerDamaged:
            onPlayerDamaged(triggerInfo);
            break;

        case PerkTrigger::OnPlayerHeal:
            onPlayerHeal(triggerInfo);
            break;

        case PerkTrigger::OnWeaponShot:
            onWeaponShot(triggerInfo);
            break;

        case PerkTrigger::OnWeaponHit:
            onWeaponHit(triggerInfo);
            break;

        case PerkTrigger::OnEnemyGotHit:
            onEnemyGotHit(triggerInfo);
            break;

        case PerkTrigger::OnEnemyKilled:
            onEnemyKilled(triggerInfo);
            break;

        case PerkTrigger::OnItemPickup:
            onItemPickup(triggerInfo);
            break;

        case PerkTrigger::OnInterval:
            break;

        default:
            std::cout << "Unknown Trigger" << std::endl;
            break;
        }
    }
}

