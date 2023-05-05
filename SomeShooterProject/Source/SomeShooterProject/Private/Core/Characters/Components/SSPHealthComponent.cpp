
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
//#include "Camera/CameraShake.h"
#include "Camera/PlayerCameraManager.h"

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

    PlayCameraShake();
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
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;
    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USSPHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
