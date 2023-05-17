#include "Core/UI/SSPBackToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SSPGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSSPBackToMenuWidget, All, All);

void USSPBackToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackToMenuButton)
	{
		BackToMenuButton->OnClicked.AddDynamic(this, &USSPBackToMenuWidget::OnBackToMenu);
	}
}

void USSPBackToMenuWidget::OnBackToMenu()
{
	if(!GetWorld()) return;

	const auto SSPGameInstance = GetWorld()->GetGameInstance<USSPGameInstance>();
	if (!SSPGameInstance) return;

	if (SSPGameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogSSPBackToMenuWidget, Error, TEXT("Menu Level Name is none!"));
		return;
	}

	UGameplayStatics::OpenLevel(this, SSPGameInstance->GetMenuLevelName());
}
