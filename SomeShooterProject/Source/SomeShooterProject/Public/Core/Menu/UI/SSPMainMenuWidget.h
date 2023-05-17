#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPCoreTypes.h"
#include "SSPMainMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USSPGameInstance;
class USSPLevelSelectorItemWidget;

UCLASS()
class SOMESHOOTERPROJECT_API USSPMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public: 

	void ShowAnim();

protected:

	UPROPERTY(meta = (BindWidget))
		UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* LevelItemBox;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* StartMenuAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* EndMenuAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> LevelSelectorItemWidgetClass;

	virtual void NativeOnInitialized() override;	
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;


private:

	UPROPERTY()
	TArray<USSPLevelSelectorItemWidget*> LevelSelectorItemWidgets;

	UFUNCTION()
		void OnStartGame();

	UFUNCTION()
		void OnQuitGame();

	void InitLevelSelectorItems();
	void OnLevelSelected(const FLevelData& NewData);
	USSPGameInstance* GetSSPGameInstance() const;
	
};
