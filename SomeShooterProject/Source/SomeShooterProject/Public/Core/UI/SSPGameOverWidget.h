#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPCoreTypes.h"
#include "SSPGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class SOMESHOOTERPROJECT_API USSPGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* Team1PlayerStatBox;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* Team2PlayerStatBox;

	UPROPERTY(meta = (BindWidget))
		UButton* RestartGameButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	virtual void NativeOnInitialized() override;

private:

	void OnMatchStateChanged(ESSPMatchState NewState);
	void UpdatePlayersStat();

	UFUNCTION()
		void OnRestartGame();
	
};
