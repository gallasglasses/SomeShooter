// Copyright Epic Games, Inc. All Rights Reserved.


#include "SomeShooterProjectGameModeBase.h"
#include "Core/Characters/SSPBasePlayerCharacter.h"
#include "Core/Characters/SSPBasePlayerController.h"
#include "Core/Characters/SSPPlayerState.h"
#include "Core/Characters/Components/SSPRespawnComponent.h"
#include "Core/UI/SSPGameHUD.h"
#include "SSPUtils.h"

#include "AIController.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSSPGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 5;

ASomeShooterProjectGameModeBase::ASomeShooterProjectGameModeBase()
{	
	DefaultPawnClass = ASSPBasePlayerCharacter::StaticClass();
    PlayerControllerClass = ASSPBasePlayerController::StaticClass();
    HUDClass = ASSPGameHUD::StaticClass();
    PlayerStateClass = ASSPPlayerState::StaticClass();
}

void ASomeShooterProjectGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();
    SetMatchState(ESSPMatchState::InProgress);
}

UClass* ASomeShooterProjectGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASomeShooterProjectGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASSPPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASSPPlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }
    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }

    StartRespawn(VictimController);
}

void ASomeShooterProjectGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool ASomeShooterProjectGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        SetMatchState(ESSPMatchState::Pause);
    }

    return PauseSet;
}

bool ASomeShooterProjectGameModeBase::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(ESSPMatchState::InProgress);
    }
    return PauseCleared;
}

void ASomeShooterProjectGameModeBase::SpawnBots()
{
    if(!GetWorld()) return;
    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto SSPAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(SSPAIController);
    }
}

void ASomeShooterProjectGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASomeShooterProjectGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASomeShooterProjectGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSSPGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

    /*const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
    RoundCountDown -= TimerRate;*/ //return float

    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASomeShooterProjectGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASomeShooterProjectGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASomeShooterProjectGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 CountBot = 0;
    int32 TeamID = 1;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
       const auto Controller = It->Get();
       if (!Controller) continue;

       const auto PlayerState = Cast<ASSPPlayerState>(Controller->PlayerState);
       if (!PlayerState) continue;

       PlayerState->SetTeamID(TeamID);
       PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
       PlayerState->SetTeamBarStyle(DetermineBarStyleByTeamID(TeamID));

       PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot" + FString::FromInt(++CountBot));
       SetPlayerColor(Controller);

       TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASomeShooterProjectGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    UE_LOG(LogSSPGameModeBase, Warning, TEXT("No color for TeamID: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

FProgressBarStyle ASomeShooterProjectGameModeBase::DetermineBarStyleByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamBarStyles.Num())
    {
        return GameData.TeamBarStyles[TeamID - 1];
    }
    UE_LOG(LogSSPGameModeBase, Warning, TEXT("No BarStyle for TeamID: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultBarStyle;
}

void ASomeShooterProjectGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    const auto Character = Cast<ASSPBasePlayerCharacter>(Controller->GetPawn());
    if (!Character) return;

    const auto PlayerState = Cast<ASSPPlayerState>(Controller->PlayerState);
    if (!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
    Character->SetHealthBarStyle(PlayerState->GetTeamBarStyle());
}

void ASomeShooterProjectGameModeBase::LogPlayerInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASSPPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        PlayerState->LogInfo();
    }
}

void ASomeShooterProjectGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if (!RespawnAvailable) return;

    const auto RespawnComponent = SSPUtils::GetPlayerComponent<USSPRespawnComponent>(Controller);
    if (!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASomeShooterProjectGameModeBase::GameOver()
{
    UE_LOG(LogSSPGameModeBase, Display, TEXT("============== GAME OVER =============="));
    LogPlayerInfo();

    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(ESSPMatchState::GameOver);
}

void ASomeShooterProjectGameModeBase::SetMatchState(ESSPMatchState State)
{
    if (MatchState == State) return;
    
    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}

