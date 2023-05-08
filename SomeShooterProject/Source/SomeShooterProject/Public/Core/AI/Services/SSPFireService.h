#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SSPFireService.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPFireService : public UBTService
{
	GENERATED_BODY()
public:

	USSPFireService();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
