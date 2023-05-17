#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSPMenuHUD.generated.h"

class USSPMainMenuWidget;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<USSPMainMenuWidget> MainMenuWidgetClass;

	virtual void BeginPlay() override;
	
};
