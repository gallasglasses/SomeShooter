#include "Core/Pickups/SSPHealthPickup.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "SSPUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASSPHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(PlayerPawn);
    if (!HealthComponent) return false;

    UE_LOG(LogHealthPickup, Display, TEXT("HealthPickup: "));
    return HealthComponent->TryToAddHealth(HealthAmount);
}
