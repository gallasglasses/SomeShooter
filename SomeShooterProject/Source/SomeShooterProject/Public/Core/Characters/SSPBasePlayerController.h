#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SSPCoreTypes.h"
#include "SSPBasePlayerController.generated.h"

class USSPRespawnComponent;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPBasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ASSPBasePlayerController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USSPRespawnComponent* RespawnComponent;
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:

	void OnPauseGame();
	void OnMatchStateChanged(ESSPMatchState State);
};
