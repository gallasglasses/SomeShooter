#include "Core/Menu/UI/SSPMainMenuWidget.h"
#include "Core/Menu/UI/SSPLevelSelectorItemWidget.h"
#include "SSPGameInstance.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSSPMainMenuWidget, All, All);

void USSPMainMenuWidget::ShowAnim()
{
	PlayAnimation(StartMenuAnimation);
}

void USSPMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USSPMainMenuWidget::OnStartGame);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USSPMainMenuWidget::OnQuitGame);
	}

	InitLevelSelectorItems();
}

void USSPMainMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if(Animation != EndMenuAnimation) return;

	const auto SSPGameInstance = GetSSPGameInstance();
	if (!SSPGameInstance) return;

	UE_LOG(LogSSPMainMenuWidget, Display, TEXT("************** OnStartGame *************"));
	UGameplayStatics::OpenLevel(this, SSPGameInstance->GetStartLevel().LevelName);
}

void USSPMainMenuWidget::OnStartGame()
{
	PlayAnimation(EndMenuAnimation);
}

void USSPMainMenuWidget::OnQuitGame()
{
	UE_LOG(LogSSPMainMenuWidget, Display, TEXT("************** OnQuitGame *************"));
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USSPMainMenuWidget::InitLevelSelectorItems()
{
	const auto SSPGameInstance = GetSSPGameInstance();
	if (!SSPGameInstance) return;

	checkf(SSPGameInstance->GetLevelsData().Num() != 0, TEXT("LevelsData musn't be empty!"));

	if (!LevelItemBox) return;
	LevelItemBox->ClearChildren();

	for (auto LevelData : SSPGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<USSPLevelSelectorItemWidget>(GetWorld(), LevelSelectorItemWidgetClass);
		if(!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USSPMainMenuWidget::OnLevelSelected);

		LevelItemBox->AddChild(LevelItemWidget);
		LevelSelectorItemWidgets.Add(LevelItemWidget);
	}

	if (SSPGameInstance->GetStartLevel().LevelName.IsNone())
	{
		OnLevelSelected(SSPGameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelected(SSPGameInstance->GetStartLevel());
	}
}

void USSPMainMenuWidget::OnLevelSelected(const FLevelData& NewData)
{
	const auto SSPGameInstance = GetSSPGameInstance();
	if (!SSPGameInstance) return;
	
	SSPGameInstance->SetStartLevel(NewData);

	for (auto LevelSelectorItemWidget : LevelSelectorItemWidgets)
	{
		if (true)
		{
			const auto IsLevelSelected = NewData.LevelName == LevelSelectorItemWidget->GetLevelData().LevelName;
			LevelSelectorItemWidget->SetSelected(IsLevelSelected);
		}
	}
}

USSPGameInstance* USSPMainMenuWidget::GetSSPGameInstance() const
{
	if (!GetWorld()) return nullptr;

	return GetWorld()->GetGameInstance<USSPGameInstance>();
}
