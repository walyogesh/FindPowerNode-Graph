#ifndef WEAPON_H
#define WEAPON_H

#include "PowerUp.h"
class WeaponSound;
class Power;
enum class WeaponType
{
    Default = 0,
    WEAPON1 = 1,
    WEAPON2 = 2,
    WEAPON3 = 3,
    Count   = 4,
    Invalid = 5
};

class Weapon : public PowerUp
{
public:
    Weapon(const char* name, const Vertex& position)
        : PowerUp(name, position, PowerUpType::WEAPON)
    {
    }

    void run();
    void reload();

private:
    WeaponType mWeaponType;
    WeaponSound* mWeaponSound;
    float mRange;// range of weapon to kill/shoot
    int mAccuracy; // Accuracy of the weapon whiling aiming the target
    int mShootSpeed;
    int mReloadFrequency;
};

#endif // WEAPON_H

