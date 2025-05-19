#pragma once

#include "ItemBaseClasses.h"
#include "Projectiles_Pistol.h"

class Pistol : public Weapon {
private:
    static constexpr float BASE_DAMAGE = 0.2f;
    static constexpr float BASE_COOLDOWN = 0.2f;

public:
    Pistol()
        : Weapon(
            ItemInfo("Pistol", "Basic weapon, semi automatic"),
            BASE_DAMAGE,
            std::make_unique<PS_Pistol>(BASE_DAMAGE),
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
