#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SSPHealthPercentDecorator.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPHealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:

	USSPHealthPercentDecorator();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float HealthPercent = 0.6f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
