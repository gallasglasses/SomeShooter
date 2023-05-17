#include "Core/Menu/UI/SSPMenuHUD.h"
#include "Core/Menu/UI/SSPMainMenuWidget.h"

void ASSPMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		const auto MainMenuWidget = CreateWidget<USSPMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			MainMenuWidget->ShowAnim();
		}
	}
}
