#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Pistol.h"

class Rifle_Burst : public Weapon {
private:
    static constexpr float BASE_DAMAGE = 1.0f;
    static constexpr float BASE_COOLDOWN = 0.8f;

    static constexpr float BURST_INTERVAL = 0.1f;
    static constexpr int BURST_SIZE = 3;
    int burstLeft = 0;
    
    ValueBar burstCooldown = ValueBar(BURST_INTERVAL);

public:
    Rifle_Burst()
        : Weapon(
            ItemInfo("Burst Rifle", "Basic weapon, shoots in bursts"),
            BASE_DAMAGE,
            std::make_unique<PS_Pistol>(BASE_DAMAGE), // Replace with PS_Burst if needed
            std::make_unique<ValueBar>(BASE_COOLDOWN)
        )
    { }

    UseResult activate(const ItemUse& use) override {
        switch (use) {
        case ItemUse::NO_USE:
            if (burstLeft > 0) {
                if (burstCooldown.isNotEmpty()) {
                    return UseResult::FAILURE_COOLDOWN;
                }

                burstCooldown.fill_to_max();
                projSpawner->shoot();
                burstLeft--;
                return UseResult::SUCCESS;
            }
            break;

        case ItemUse::ATTACK:
            if (Weapon::activate(use) < UseResult::SUCCESS)
                break;

            projSpawner->shoot();
            burstLeft += BURST_SIZE - 1;
            return UseResult::SUCCESS;

        default:
            break;
        }

        return UseResult::FAILURE;
    }

    void tick(const float& deltaTime) override {
        if (burstLeft > 0) {
            burstCooldown.addValue(-deltaTime);
        }

        Weapon::tick(deltaTime);
    }
};
