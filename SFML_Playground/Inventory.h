#include "ItemBaseClasses.h"

// Inventory class: manages all items, ensuring that only one weapon is active at a time.
class Inventory
{
private:
    // Store all acquired weapons.
    std::vector<std::unique_ptr<Weapon>> weapons;
    int activeWeaponIndex = -1; // -1 means no active weapon

    // Store all acquired perks.
    std::vector<std::unique_ptr<Perk>> perks;

public:
    Inventory() = default;

    Inventory(std::unique_ptr<Weapon> initialWeapon)
        : Inventory()
    {
        addWeapon(std::move(initialWeapon));
    }

    ~Inventory() = default;

    size_t getNumWeapons() const
    {
        return weapons.size();
    }

    size_t getNumPerks() const
    {
        return perks.size();
    }

    // Adds a new weapon. If this is the first weapon, it becomes the active weapon.
    void addWeapon(std::unique_ptr<Weapon> weapon)
    {
        if (!weapon) 
            return;
        weapons.push_back(std::move(weapon));
        if (activeWeaponIndex == -1)
            activeWeaponIndex = 0;
    }

    // Switches the active weapon by its index.
    bool switchWeapon(size_t index)
    {
        if (index < weapons.size())
        {
            activeWeaponIndex = static_cast<int>(index);
            std::cout << "Switched active weapon to: "
                << weapons[activeWeaponIndex]->getItemInfo().name << "\n";
            return true;
        }
        return false;
    }

    // Returns a pointer to the current active weapon.
    Weapon* getActiveWeapon() const
    {
        if (activeWeaponIndex >= 0 && activeWeaponIndex < static_cast<int>(weapons.size()))
            return weapons[activeWeaponIndex].get();
        return nullptr;
    }

    // Adds a new perk.
    void addPerk(std::unique_ptr<Perk> perk)
    {
        if (!perk) 
            return;
        perks.push_back(std::move(perk));
    }

    // This function should be called when an in-game event occurs to trigger all relevant perks.
    void triggerPerks(PerkTrigger trigger)
    {
        for (auto& perk : perks)
        {
            perk->onTrigger(trigger);
        }
    }

    // Removes a weapon by index and returns the unique pointer.
    std::unique_ptr<Weapon> removeWeapon(size_t index)
    {
        if (index >= weapons.size())
            return nullptr;

        // Move the unique_ptr out of the vector.
        auto removedWeapon = std::move(weapons[index]);
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
    std::unique_ptr<Perk> removePerk(size_t index)
    {
        if (index >= perks.size())
            return nullptr;

        // Move the unique_ptr out of the vector.
        auto removedPerk = std::move(perks[index]);
        perks.erase(perks.begin() + index);
        return removedPerk;
    }
    
    void clear_weapons()
    {
        weapons.clear();
    }

    void clear_perks()
    {
        perks.clear();
    }

    void clear()
    {
        clear_perks();
        clear_weapons();
        activeWeaponIndex = -1;
    }

    // Optional: an update function to process inventory-related logic each frame.
    void update()
    {
        // For example: check if weapons are reloading or if perk effects need updating.
    }
};