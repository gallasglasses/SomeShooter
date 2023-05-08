#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSPCoreTypes.h"
#include "SSPBaseWeapon.generated.h"

class UParticleSystem;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	ASSPBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();
	virtual void StopFire();

    void ChangeClip();
    bool CanReload() const;

    FWeaponUIData GetCurrentUIData() const {return UIData;};
    FAmmoData GetCurrentAmmoData() const {return CurrentAmmo;};

    bool TryToAddAmmo(int32 ClipsAmount);
    bool IsAmmoEmpty() const;
    bool IsAmmoFull() const;

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "Muzzle";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ParticleSystem")
    UParticleSystem* MuzzleParticleSystem;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ParticleSystem")
        float ParticleScale = 0.3f;

	virtual void BeginPlay() override;
	virtual void MakeShot();

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    void MakeDamage(FHitResult& HitResult);

    ACharacter* GetPlayer() const;
    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    FVector GetMuzzleWorldLocation() const;
    FTransform GetMuzzleWorldTransform() const;

    void DecreaseAmmo();
    void LogAmmo();
    virtual void SpawnMuzzleParticle();

    bool IsClipEmpty() const;

private:
    
    FAmmoData CurrentAmmo;

};
