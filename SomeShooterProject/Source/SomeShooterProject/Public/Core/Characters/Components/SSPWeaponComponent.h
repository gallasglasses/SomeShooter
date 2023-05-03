#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSPCoreTypes.h"
#include "SSPWeaponComponent.generated.h"

class ASSPBaseWeapon;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOMESHOOTERPROJECT_API USSPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USSPWeaponComponent();

    void StartFire();
    void StopFire();
    void NextWeapon();
    void Reload();

    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
    bool GetCurrentWeaponType(ECharacterWeapon& WeaponType) const;
    bool TryToAddAmmo(TSubclassOf<ASSPBaseWeapon> WeaponType, int32 ClipsAmount);
    
protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TMap <ECharacterWeapon, FWeaponData> WeaponsData;
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
        FName WeaponEquipSocketName = "GunSocket";   //FName WeaponEquipSocketName = "Muzzle";
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
        FName WeaponArmorySocketName = "PelvisSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimMontage")
        UAnimMontage* EquipAnimMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimMontage")
        UAnimMontage* UnequipAnimMontage;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void MakeShot();
    
private:

    UPROPERTY()
    ASSPBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TMap <ECharacterWeapon, ASSPBaseWeapon*> MapWeapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    ECharacterWeapon CurrentWeaponType = ECharacterWeapon::Riffle;

    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    void SpawnWeapons();
    void EquipWeapon(ECharacterWeapon WeaponType);
    void AttachWeaponToSocket(ASSPBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    bool CanDoAction() const;

    void OnEmptyClip(ASSPBaseWeapon* EmptyWeapon);
    void ChangeClip();

};
