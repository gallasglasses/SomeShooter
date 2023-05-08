#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SSPNeedAmmoDecorator.generated.h"

class ASSPBaseWeapon;

UCLASS()
class SOMESHOOTERPROJECT_API USSPNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:

	USSPNeedAmmoDecorator();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TSubclassOf<ASSPBaseWeapon> WeaponType;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
