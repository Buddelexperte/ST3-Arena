#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Shotgun.h"

class Shotgun : public Weapon {
private:
    static constexpr float BASE_DAMAGE = 0.2f;
    static constexpr float BASE_COOLDOWN = 1.0f;

public:
    Shotgun()
        : Weapon(
            ItemInfo("Shotgun", "Basic weapon, spread pattern"),
            BASE_DAMAGE,
            std::make_unique<PS_Shotgun>(BASE_DAMAGE),
            std::make_unique<ValueBar>(BASE_COOLDOWN)
        )
    { }

    UseResult activate(const ItemUse& use) override {
        switch (use) {
        case ItemUse::ATTACK:
            if (Weapon::activate(use) < UseResult::SUCCESS)
                break;

            projSpawner->shoot();
            return UseResult::SUCCESS;
        default:
            break;
        }

        return UseResult::FAILURE;
    }
};
