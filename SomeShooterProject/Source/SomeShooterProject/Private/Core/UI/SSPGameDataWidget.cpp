#include "Core/UI/SSPGameDataWidget.h"
#include "SomeShooterProjectGameModeBase.h"
#include "Core/Characters/SSPPlayerState.h"

int32 USSPGameDataWidget::GetKillsNum() const
{
    const auto PlayerState = GetSSPPlayerState();
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USSPGameDataWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetSSPGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 USSPGameDataWidget::GetTotalRoundsNum() const
{
    const auto GameMode = GetSSPGameMode();
    return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 USSPGameDataWidget::GetRoundSecondsRemaining() const
{
    const auto GameMode = GetSSPGameMode();
    return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

ASomeShooterProjectGameModeBase* USSPGameDataWidget::GetSSPGameMode() const
{
    return GetWorld() ? Cast<ASomeShooterProjectGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASSPPlayerState* USSPGameDataWidget::GetSSPPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASSPPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
