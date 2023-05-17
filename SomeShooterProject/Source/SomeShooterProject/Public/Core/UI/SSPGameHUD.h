#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSPCoreTypes.h"
#include "SSPGameHUD.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API ASSPGameHUD : public AHUD
{
	GENERATED_BODY()
	
	public:

		virtual void DrawHUD() override;

	protected:

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
			TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
			TSubclassOf<UUserWidget> PauseWidgetClass;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
			TSubclassOf<UUserWidget> GameOverWidgetClass;

		virtual void BeginPlay() override;

	private:
		
		UPROPERTY()
			TMap<ESSPMatchState, UUserWidget*> GameWidgets;

		UPROPERTY()
			UUserWidget* CurrentWidget = nullptr;

		void DrawCrossHair();
		void OnMatchStateChanged(ESSPMatchState NewState);
};
