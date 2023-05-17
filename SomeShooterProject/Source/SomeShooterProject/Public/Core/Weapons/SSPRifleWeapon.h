#pragma once

#include "CoreMinimal.h"
#include "Core/Weapons/SSPBaseWeapon.h"
#include "SSPRifleWeapon.generated.h"

class USSPWeaponFXComponent;
class UNiagaraSystem;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPRifleWeapon : public ASSPBaseWeapon
{
	GENERATED_BODY()

public:

    ASSPRifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;


    virtual void Zoom(bool Enabled) override;

protected:

    /*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "Muzzle";*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float FOVZoomAngle = 50.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VFX")
    USSPWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;
	virtual void MakeShot() override;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:

    FTimerHandle ShotTimerHandle;

    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

    float DefaultCameraFOV = 90.f;
};
