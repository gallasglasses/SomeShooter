#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SSPMenuPlayerController.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API ASSPMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;
};
