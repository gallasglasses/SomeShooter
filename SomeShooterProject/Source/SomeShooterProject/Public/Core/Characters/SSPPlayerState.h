#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SSPPlayerState.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API ASSPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	void SetTeamID(int32 ID) { TeamID = ID; }
	int32 GetTeamID() const { return TeamID; }

	void SetTeamColor(FLinearColor Color) { TeamColor = Color; }
	FLinearColor GetTeamColor() const { return TeamColor; }

	void SetTeamBarStyle(FProgressBarStyle BarStyle) { TeamBarStyle = BarStyle; }
	FProgressBarStyle GetTeamBarStyle() const { return TeamBarStyle; }

	void AddKill() { ++KillsNum; }
	int32 GetKillsNum() const { return KillsNum; }

	void AddDeath() { ++DeathsNum; }
	int32 GetDeathsNum() const { return DeathsNum; }

	void LogInfo();

private:
	
	int32 TeamID;
	FString PlayerName;
	FLinearColor TeamColor;
	FProgressBarStyle TeamBarStyle;

	int32 KillsNum = 0;
	int32 DeathsNum = 0;
};
