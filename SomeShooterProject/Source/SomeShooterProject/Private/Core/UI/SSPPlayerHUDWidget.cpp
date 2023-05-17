#include "Core/UI/SSPPlayerHUDWidget.h"
#include "SSPUtils.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

float USSPPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.f;

    return HealthComponent->GetHealthPercent();
}

float USSPPlayerHUDWidget::GetHealth() const
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.f;

    return HealthComponent->GetHealth();
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

void USSPPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USSPPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void USSPPlayerHUDWidget::OnHealthChanged(float Health, float DeltaHealth)
{
    if (DeltaHealth < 0)
    {
        //OnTakeDamage();

        if (!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    //UpdateHealthBar();
}

void USSPPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(NewPawn);
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USSPPlayerHUDWidget::OnHealthChanged);
    }
    //UpdateHealthBar();
}

void USSPPlayerHUDWidget::UpdateHealthBar()
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(GetHealthPercent());
    }

    if (HealthText)
    {
        HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), GetHealth())));
    }
}
