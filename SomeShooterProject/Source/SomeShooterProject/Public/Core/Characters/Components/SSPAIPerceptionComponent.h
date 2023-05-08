#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "SSPAIPerceptionComponent.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:

	AActor* GetClosestEnemy() const;
	
};
