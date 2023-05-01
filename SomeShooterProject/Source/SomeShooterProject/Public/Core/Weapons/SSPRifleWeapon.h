#pragma once

#include "CoreMinimal.h"
#include "Core/Weapons/SSPBaseWeapon.h"
#include "SSPRifleWeapon.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API ASSPRifleWeapon : public ASSPBaseWeapon
{
	GENERATED_BODY()

public:

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "Muzzle";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

	virtual void MakeShot() override;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    
    FVector GetMuzzleWorldLocation() const;

private:
    FTimerHandle ShotTimerHandle;
    
};
