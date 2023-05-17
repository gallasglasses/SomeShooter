#include "Core/Characters/Components/SSPRespawnComponent.h"
#include "SomeShooterProjectGameModeBase.h"

USSPRespawnComponent::USSPRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USSPRespawnComponent::Respawn(int32 RespawnTime)
{
	if(!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USSPRespawnComponent::RespawnTimerUpdate, 1.f, true);
}

bool USSPRespawnComponent::IsRespawnInProgress() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void USSPRespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountDown == 0 )
	{
		if (!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<ASomeShooterProjectGameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}
