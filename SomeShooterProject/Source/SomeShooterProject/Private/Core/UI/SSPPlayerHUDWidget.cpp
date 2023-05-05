#include "Core/UI/SSPPlayerHUDWidget.h"
#include "SSPUtils.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"

float USSPPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.f;

    return HealthComponent->GetHealthPercent();
}

bool USSPPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool USSPPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(GetOwningPlayerPawn());
  
    return HealthComponent && !HealthComponent->IsDead();
}

bool USSPPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();

    
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool USSPPlayerHUDWidget::Initialize()
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(GetOwningPlayerPawn());
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USSPPlayerHUDWidget::OnHealthChanged);
    }

    return Super::Initialize();
}

void USSPPlayerHUDWidget::OnHealthChanged(float Health, float DeltaHealth)
{
    if (DeltaHealth < 0)
    {
        OnTakeDamage();
    }
}
