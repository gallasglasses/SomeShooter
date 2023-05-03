#pragma once

#include "CoreMinimal.h"
#include "Core/Pickups/SSPBasePickup.h"
#include "SSPHealthPickup.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API ASSPHealthPickup : public ASSPBasePickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
		int32 HealthAmount = 10;

private:

	virtual bool GivePickupTo(APawn* PlayerPawn) override;
	
};
