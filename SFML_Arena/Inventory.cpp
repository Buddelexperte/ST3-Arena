#pragma once

#include "Inventory.h" // Own header file
#include "Player.h"
#include "GameInstance.h"

Inventory::Inventory(Player* playerRef)
	: owner(playerRef),
    levelSystem(this), // Initialize the level system with a reference to this inventory
    hurtTimer(DEFAULT_HURT_FREQ),
	startWeaponName("StartWeapon") // Default starting weapon name
{

}

// Adds a new weapon. If this is the first weapon, it becomes the active weapon.
void Inventory::addWeapon(std::unique_ptr<Weapon> weapon)
{
    if (!weapon)
        return;
    
    weapon->setOwningInventory(this);
    weapons.push_back(std::move(weapon));
    if (activeWeaponIndex == -1)
        activeWeaponIndex = 0;
}

std::string Inventory::getStartWeapon() const
{
    return startWeaponName;
}

void Inventory::setStartWeapon(const std::string& newWeaponName)
{
    startWeaponName = newWeaponName;
}

void Inventory::addWeapon_byName(const std::string& name)
{
    std::unique_ptr<Weapon> weapon = makeWeapon(name);

    addWeapon(std::move(weapon));
}

// Switches the active weapon by its index.
bool Inventory::selectWeapon(size_t index)
{
    if (index < weapons.size())
    {
        activeWeaponIndex = static_cast<int>(index);
        std::cout << "Switched active weapon to: "
            << weapons[activeWeaponIndex]->getItemInfo().name << "\n";
        weapons[activeWeaponIndex]->setRenderInfo(owner->getRenderInfo());
        return true;
    }
    return false;
}

UseResult Inventory::shootWeapon()
{
    Weapon* activeWeapon = getActiveWeapon();

    if (activeWeapon == nullptr)
        return UseResult::FAILURE;

    UseResult result = activeWeapon->activate(ItemUse::ATTACK);

    if (result >= UseResult::SUCCESS)
    {
        PerkTriggerInfo triggerInfo(PerkTrigger::OnWeaponShot, owner->getPosition());
        triggerPerks(triggerInfo);
    }

    return result;
}

UseResult Inventory::loadUpWeapon()
{
    Weapon* activeWeapon = getActiveWeapon();

    if (activeWeapon == nullptr)
        return UseResult::FAILURE;

    return activeWeapon->activate(ItemUse::LOAD_UP);
}

UseResult Inventory::loadUpWeapon_cancel()
{
    Weapon* activeWeapon = getActiveWeapon();

    if (activeWeapon == nullptr)
        return UseResult::FAILURE;

    return activeWeapon->activate(ItemUse::CANCEL_LOAD);
}

// Returns a pointer to the current active weapon.
Weapon* Inventory::getActiveWeapon() const
{
    if (activeWeaponIndex >= 0 && activeWeaponIndex < static_cast<int>(weapons.size()))
        return weapons[activeWeaponIndex].get();
    return nullptr;
}

Player* Inventory::getOwner()
{
    return owner;
}


void Inventory::addPerk_byTag(const std::string& tag)
{
    std::unique_ptr<Perk> perk = makePerk(tag);

    applyPerk(std::move(perk));
}

// Adds a new perk.
void Inventory::applyPerk(std::unique_ptr<Perk> perk)
{
    if (!perk)
        return;

    Perk* perk_ptr = perk.get();

    perks.push_back(std::move(perk));

    perk_ptr->setOwningInventory(this);
    perk_ptr->activate(ItemUse::ON_EQUIP);
}

// This function should be called when an in-game event occurs to trigger all relevant perks.
void Inventory::triggerPerks(PerkTriggerInfo& triggerInfo)
{
    for (auto& perk : perks)
    {
        if (perk->hasTrigger(triggerInfo.trigger))
            perk->tryTrigger(triggerInfo);
    }
}

// Removes a weapon by index and returns the unique pointer.
std::unique_ptr<Weapon> Inventory::removeWeapon(size_t index)
{
    if (index >= weapons.size())
        return nullptr;

    // Move the unique_ptr out of the vector.
    std::unique_ptr<Weapon> removedWeapon = std::move(weapons[index]);
    weapons.erase(weapons.begin() + index);

    // Update the active weapon index.
    if (activeWeaponIndex == static_cast<int>(index))
    {
        activeWeaponIndex = weapons.empty() ? -1 : 0;
    }
    else if (activeWeaponIndex > static_cast<int>(index))
    {
        activeWeaponIndex--;
    }
    return removedWeapon;
}

// Removes a perk by index and returns the unique pointer.
std::unique_ptr<Perk> Inventory::removePerk(size_t index)
{
    if (index >= perks.size())
        return nullptr;

    // Move the unique_ptr out of the vector.
    std::unique_ptr<Perk> removedPerk = std::move(perks[index]);
    perks.erase(perks.begin() + index);
    return removedPerk;
}

void Inventory::clear_weapons()
{
    weapons.clear();
    activeWeaponIndex = -1;
}

void Inventory::clear_perks()
{
    perks.clear();
}

void Inventory::clear_inventory()
{
    clear_perks();
    clear_weapons();
}

void Inventory::reset()
{
	levelSystem.reset();
    clear_inventory();
    // Add wepaon by StartWeaponName
    addWeapon_byName(startWeaponName);

    cooldownMultiplier  = DEFAULT_COOLDOWN_MULTIPLIER;
    cooldownSubtractor  = DEFAULT_COOLDOWN_SUBTRACTOR;
    hurtBias            = DEFAULT_HURT_BIAS;
    hurtMultiplier      = DEFAULT_HURT_MULTIPLIER;
    magneticDistance    = DEFAULT_MAGNETIC_DISTANCE;
    setHurtFreq(DEFAULT_HURT_FREQ);

    numLevelUpsQueued   = 0;
}

// Optional: an update function to process inventory-related logic each frame.
void Inventory::tick(const float& deltaTime)
{
    hurtTimer.addValue(-deltaTime);

    getActiveWeapon()->tick(deltaTime);

    // For example: check if weapons are reloading or if perk effects need updating.
    static PerkTriggerInfo intervalTriggerInfo(PerkTrigger::OnInterval);

    for (auto& perk : perks)
    {
        if (perk->hasTrigger(intervalTriggerInfo.trigger))
            perk->onInterval(deltaTime);
    }
}

LevelSystem& Inventory::getLevelSystem()
{
    return levelSystem;
}

void Inventory::addQueuedLevelUp()
{
    numLevelUpsQueued++;
}

void Inventory::removeQueuedLevelUp()
{
    if (numLevelUpsQueued > 0)
    {
        numLevelUpsQueued--;
    }
}

unsigned int Inventory::getNumQueuedLevelUps() const
{
    return numLevelUpsQueued;
}

size_t Inventory::getNumWeapons() const
{
    return weapons.size();
}

size_t Inventory::getNumPerks() const
{
    return perks.size();
}

void Inventory::setInvincible(const bool& newVal)
{
    bIsInvincible = newVal;
}

bool Inventory::getIsInvincible() const
{
    return bIsInvincible;
}

void Inventory::setHurtFreq(const float& newVal)
{
    hurtTimer.setMaxValue(newVal);
}

void Inventory::fillHurtFreq()
{
    hurtTimer.fill_to_max();
}

void Inventory::setTimedInvincibility(const float& delay)
{
    hurtTimer.setValue(delay);
}

void Inventory::removeTimedInvincibility()
{
    hurtTimer.zero_value();
}

float Inventory::getHurtFreq() const
{
    return hurtTimer.getMaxValue();
}

bool Inventory::canBeHurt() const
{
    return hurtTimer.isEmpty();
}

void Inventory::setCooldownMultiplier(const float& newMulti)
{
    cooldownMultiplier = newMulti;
}

float Inventory::getCooldownMultiplier() const
{
    return cooldownMultiplier;
}

void Inventory::setCooldownSubtractor(const float& newSub)
{
    cooldownSubtractor = newSub;
}

float Inventory::getCooldownSubtractor() const
{
    return cooldownSubtractor;
}

void Inventory::setHurtBias(const float& val)
{
    hurtBias = val;
}

void Inventory::applyHurtBias(const float& val)
{
    hurtBias += val;
}

float Inventory::getHurtBias() const
{
    return hurtBias;
}

void Inventory::setHurtMultiplier(const float& val)
{
    hurtMultiplier = val;
}

void Inventory::applyHurtMultiplier(const float& val)
{
    hurtMultiplier *= val;
}

float Inventory::getHurtMultiplier() const
{
    return hurtMultiplier;
}

void Inventory::setCritChance(const float& val)
{
    critChance = val;
}

void Inventory::addCritChance(const float& val)
{
    critChance += val;
}

float Inventory::getCritChance() const
{
    return critChance;
}

void Inventory::applyCrit(float& damage)
{
    float randVal = RNG::floatInRange(0.0f, 1.0f);

    if (randVal < critChance)
    {
        std::cout << "CRITICAL HIT" << std::endl;
        damage *= CRIT_MULITPLIER;
    }
}

void Inventory::setMagneticRange(const float& newRange)
{
    magneticDistance = newRange;
}

float Inventory::getMagneticRange() const
{
    return magneticDistance;
}

