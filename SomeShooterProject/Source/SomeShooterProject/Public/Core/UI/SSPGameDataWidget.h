#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSPGameDataWidget.generated.h"

class ASomeShooterProjectGameModeBase;
class ASSPPlayerState;

UCLASS()
class SOMESHOOTERPROJECT_API USSPGameDataWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentRoundNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTotalRoundsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetRoundSecondsRemaining() const;

private: 

	ASomeShooterProjectGameModeBase* GetSSPGameMode() const;
	ASSPPlayerState* GetSSPPlayerState() const;

};
