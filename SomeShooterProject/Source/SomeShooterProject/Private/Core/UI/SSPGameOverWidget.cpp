#include "Core/UI/SSPGameOverWidget.h"
#include "SomeShooterProjectGameModeBase.h"
#include "Core/Characters/SSPPlayerState.h"
#include "Core/UI/SSPPlayerStatRowWidget.h"
#include "SSPUtils.h"

#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void USSPGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<ASomeShooterProjectGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &USSPGameOverWidget::OnMatchStateChanged);
        }
    }

    if (RestartGameButton)
    {
        RestartGameButton->OnClicked.AddDynamic(this, &USSPGameOverWidget::OnRestartGame);
    }
}

void USSPGameOverWidget::OnMatchStateChanged(ESSPMatchState NewState)
{
    if (NewState == ESSPMatchState::GameOver)
    {
        UpdatePlayersStat();
    }
}

void USSPGameOverWidget::UpdatePlayersStat()
{
    if(!GetWorld() || !Team1PlayerStatBox && !Team2PlayerStatBox) return;

    Team1PlayerStatBox->ClearChildren();
    Team2PlayerStatBox->ClearChildren();

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto PlayerState = Cast<ASSPPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        const auto PlayerStatRowWidget = CreateWidget<USSPPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget) continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(SSPUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(SSPUtils::TextFromInt(PlayerState->GetDeathsNum()));
        //PlayerStatRowWidget->SetTeam(SSPUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
        
        if (PlayerState->GetTeamID() == 1)
        {
            Team1PlayerStatBox->AddChild(PlayerStatRowWidget);
            PlayerStatRowWidget->SetPlayerIndicatorColor(PlayerState->GetTeamID());
        }
        else
        {
            Team2PlayerStatBox->AddChild(PlayerStatRowWidget);
            PlayerStatRowWidget->SetPlayerIndicatorColor(PlayerState->GetTeamID());
        }
    }
}

void USSPGameOverWidget::OnRestartGame()
{
    //const FName CurrentLevelName = "TestLevel";
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
