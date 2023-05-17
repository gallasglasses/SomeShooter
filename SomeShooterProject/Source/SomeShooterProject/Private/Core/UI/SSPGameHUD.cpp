#include "Core/UI/SSPGameHUD.h"
#include "SomeShooterProjectGameModeBase.h"

#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSPGameHUD, All, All);

void ASSPGameHUD::DrawHUD() 
{
    Super::DrawHUD();
    //DrawCrossHair();
}

void ASSPGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(ESSPMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESSPMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESSPMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair:GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if(!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ASomeShooterProjectGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASSPGameHUD::OnMatchStateChanged);
        }
    }
}

void ASSPGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Black;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASSPGameHUD::OnMatchStateChanged(ESSPMatchState NewState)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GameWidgets.Contains(NewState))
    {
        CurrentWidget = GameWidgets[NewState];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }

    UE_LOG(LogSPGameHUD, Display, TEXT("Match State Changed: %s"), *UEnum::GetValueAsString(NewState));
    
}
