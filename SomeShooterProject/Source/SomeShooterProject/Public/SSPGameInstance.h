#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SSPCoreTypes.h"
#include "SSPGameInstance.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	FLevelData GetStartLevel() const { return StartLevel; }
	void SetStartLevel(const FLevelData& LevelData) { StartLevel = LevelData; }

	TArray<FLevelData> GetLevelsData() const { return LevelsData; }

	FName GetMenuLevelName() const { return MenuLevelName; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
		TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
		FName MenuLevelName = NAME_None;

private:

	FLevelData StartLevel;

};
