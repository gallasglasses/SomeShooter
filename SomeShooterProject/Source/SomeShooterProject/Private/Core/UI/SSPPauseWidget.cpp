#include "Core/UI/SSPPauseWidget.h"

#include "Components/Button.h"
#include "Gameframework/GameModeBase.h"

void USSPPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized(); 
    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &USSPPauseWidget::OnContinue);
    }
}

void USSPPauseWidget::OnContinue()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}
