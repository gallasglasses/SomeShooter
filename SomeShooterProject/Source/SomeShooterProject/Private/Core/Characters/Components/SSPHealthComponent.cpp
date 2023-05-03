
#include "Core/Characters/Components/SSPHealthComponent.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All);

USSPHealthComponent::USSPHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool USSPHealthComponent::TryToAddHealth(int32 HealthAmount)
{
    if (IsDead() || IsHealthFull() || HealthAmount <= 0) return false;
    SetHealth(Health + HealthAmount);
    return true;
}

void USSPHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    check(MaxHealth > 0);
    SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
	{
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USSPHealthComponent::OnTakeAnyDamage);
	}
}

bool USSPHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USSPHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);


	if (IsDead())
	{
        OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USSPHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

}

void USSPHealthComponent::HealUpdate() 
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull() && GetWorld())
	{
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USSPHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f , MaxHealth);
    OnHealthChanged.Broadcast(Health);
}