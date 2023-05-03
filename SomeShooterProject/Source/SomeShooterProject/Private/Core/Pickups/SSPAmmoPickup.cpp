#include "Core/Pickups/SSPAmmoPickup.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "SSPUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASSPAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(PlayerPawn);
    if(!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(PlayerPawn);
    if (!WeaponComponent) return false;

    UE_LOG(LogAmmoPickup, Display, TEXT("AmmoPickup: "));
    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
