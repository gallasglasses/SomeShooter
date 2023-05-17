#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSPRespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOMESHOOTERPROJECT_API USSPRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USSPRespawnComponent();

	void Respawn(int32 RespawnTime);
	int32 GetRespawnCountDown() const { return RespawnCountDown;}
	bool IsRespawnInProgress() const;

private:	

	FTimerHandle RespawnTimerHandle;
	int32 RespawnCountDown = 0;

	void RespawnTimerUpdate();
		
};
