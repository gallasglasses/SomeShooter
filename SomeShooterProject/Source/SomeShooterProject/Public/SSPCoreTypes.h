#pragma once

#include "SSPCoreTypes.generated.h"

class ASSPBaseWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASSPBaseWeapon*);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
        int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        bool Infinite;
};

UENUM(BlueprintType)
enum class ECharacterWeapon : uint8
{
    Riffle = 0,
    ShotGun
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
        TSubclassOf<ASSPBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimMontage")
        bool HasReloadAnimMontage = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimMontage", meta = (EditCondition = "HasReloadAnimMontage"))
        UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
        UTexture2D* CrossHairIcon;
};