#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPSpectatorWidget.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPSpectatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetRespawnTime(int32& CountDownTime) const;
};
