#include "Core/Characters/Components/SSPAIWeaponComponent.h"
#include "Core/Weapons/SSPBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIWeaponComponent, All, All);

void USSPAIWeaponComponent::StartFire()
{
    if (!CanDoAction())
    {
        UE_LOG(LogAIWeaponComponent, Display, TEXT("Can't Do Action!"));
        return;
    }
    if (CurrentWeapon->IsAmmoEmpty())
    {
        UE_LOG(LogAIWeaponComponent, Display, TEXT("Ammo Is Empty | Next Weapon!"));
        NextWeapon();
    }
    else
    {
        UE_LOG(LogAIWeaponComponent, Display, TEXT("Can Do Action!"));
        CurrentWeapon->StartFire();
    }
}

void USSPAIWeaponComponent::NextWeapon()
{
    if (!CanDoAction())
    {
        UE_LOG(LogAIWeaponComponent, Display, TEXT("Can't Equip Next Weapon Now!"));
        return;
    }
    // It's done for only two types of weapons!!!
    ECharacterWeapon NextWeaponType = CurrentWeaponType == ECharacterWeapon::Riffle ? ECharacterWeapon::ShotGun : ECharacterWeapon::Riffle;
    
    if (CurrentWeaponType == ECharacterWeapon::Riffle)
    {
        const auto NextWeapon = *MapWeapons.Find(NextWeaponType);
        if (NextWeapon->IsAmmoEmpty()) return;
    }
    //const auto NextWeapon = *MapWeapons.Find(NextWeaponType);
    //if (NextWeapon->IsAmmoEmpty()) return;

    if (NextWeaponType != CurrentWeaponType)
    {
        CurrentWeaponType = NextWeaponType;
        EquipWeapon(CurrentWeaponType);
    }
}
