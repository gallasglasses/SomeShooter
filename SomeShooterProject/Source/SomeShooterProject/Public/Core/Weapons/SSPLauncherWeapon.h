#pragma once

#include "CoreMinimal.h"
#include "Core/Weapons/SSPBaseWeapon.h"
#include "SSPLauncherWeapon.generated.h"

class ASSPProjectile;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPLauncherWeapon : public ASSPBaseWeapon
{
	GENERATED_BODY()

public:

    virtual void StartFire() override;

protected:

   /* UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
        FName MuzzleSocketName = "Muzzle_01";*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TSubclassOf<ASSPProjectile> ProjectileClass;

    virtual void MakeShot() override;
};
