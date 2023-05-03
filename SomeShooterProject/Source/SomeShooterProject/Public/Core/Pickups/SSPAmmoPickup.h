#pragma once

#include "CoreMinimal.h"
#include "Core/Pickups/SSPBasePickup.h"
#include "SSPAmmoPickup.generated.h"

class ASSPBaseWeapon;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPAmmoPickup : public ASSPBasePickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "20.0"))
		int32 ClipsAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		TSubclassOf<ASSPBaseWeapon> WeaponType;
	
private:

	virtual bool GivePickupTo(APawn* PlayerPawn) override;
	
};
