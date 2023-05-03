#include "Core/UI/SSPEquippedItemsWidget.h"
#include "SSPUtils.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"

bool USSPEquippedItemsWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool USSPEquippedItemsWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool USSPEquippedItemsWidget::GetCurrentWeaponType(ECharacterWeapon& WeaponType) const
{
    const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponType(WeaponType);
}