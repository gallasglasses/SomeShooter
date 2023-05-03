#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSPCoreTypes.h"
#include "SSPBaseWeapon.generated.h"

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

protected:

    /*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "Muzzle";*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

	virtual void BeginPlay() override;
	virtual void MakeShot();

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    //virtual FVector GetMuzzleWorldLocation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    void MakeDamage(FHitResult& HitResult);

    ACharacter* GetPlayer() const;
    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void LogAmmo();

private:
    
    FAmmoData CurrentAmmo;

};
