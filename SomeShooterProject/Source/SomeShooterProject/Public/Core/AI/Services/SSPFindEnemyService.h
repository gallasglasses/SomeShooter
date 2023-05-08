#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SSPFindEnemyService.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPFindEnemyService : public UBTService
{
	GENERATED_BODY()

public: 
	
	USSPFindEnemyService();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
