#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSPCoreTypes.h"
#include "SSPWeaponComponent.generated.h"

class ASSPBaseWeapon;
class UAnimMontage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipLauncher, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOMESHOOTERPROJECT_API USSPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USSPWeaponComponent();

    FOnEquipLauncher OnEquipLauncher;

    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();

    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
    bool GetCurrentWeaponType(ECharacterWeapon& WeaponType) const;
    bool TryToAddAmmo(TSubclassOf<ASSPBaseWeapon> WeaponType, int32 ClipsAmount);
    bool NeedAmmo(TSubclassOf<ASSPBaseWeapon> WeaponType);
    
    void Zoom(bool Enabled);

protected:

    UPROPERTY()
        ASSPBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
        TMap <ECharacterWeapon, ASSPBaseWeapon*> MapWeapons;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
        UAnimMontage* LauncherEquipedAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
        ECharacterWeapon CurrentWeaponType = ECharacterWeapon::Riffle;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void MakeShot();
    void EquipWeapon(ECharacterWeapon WeaponType);

    bool CanDoAction() const;
    
private:

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;
    bool bIsEquipLauncher = false;

    void AttachWeaponToSocket(ASSPBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void ChangeClip();
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);
    void OnEquipedLauncher(USkeletalMeshComponent* MeshComp);
    void OnEmptyClip(ASSPBaseWeapon* EmptyWeapon);
    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void SpawnWeapons();
};
