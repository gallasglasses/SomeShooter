#include "Core/Characters/SSPPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSSPPlayerState, All, All);

void ASSPPlayerState::LogInfo()
{
	UE_LOG(LogSSPPlayerState, Display, TEXT("Player Name: %s, TeamID: %i, Kills: %i, Deaths: %i"), *GetPlayerName(), TeamID, KillsNum, DeathsNum);
}
