#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "Core/Weapons/SSPBaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Core/Animations/SSPEquipFinishedAnimNotify.h"
#include "Core/Animations/SSPReloadFinishedAnimNotify.h"
#include "Core/Animations/SSPAnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

constexpr static int32 WeaponNum = 2; //Character can hold only 2 weapon items!

USSPWeaponComponent::USSPWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USSPWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    checkf(WeaponsData.Num() == WeaponNum, TEXT("Character can hold only %i weapon items!"), WeaponNum);

    InitAnimations();
	SpawnWeapons();
    EquipWeapon(CurrentWeaponType);
}

void USSPWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto &Weapon : MapWeapons)
    {
        Weapon.Value->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon.Value->Destroy();
    }
    MapWeapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USSPWeaponComponent::SpawnWeapons() 
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character && !GetWorld())
    {
        return;
    }

    for (const auto &OneWeaponData : WeaponsData)
    {
        auto Weapon = GetWorld()->SpawnActor<ASSPBaseWeapon>((OneWeaponData.Value).WeaponClass);
        if (!Weapon) continue;

        Weapon->OnClipEmpty.AddUObject(this, &USSPWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        //Weapons.Add(Weapon);
        MapWeapons.Add(OneWeaponData.Key, Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponEquipSocketName);
    }
}

void USSPWeaponComponent::AttachWeaponToSocket(ASSPBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if(!Weapon || !SceneComponent) return;

    FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentTransformRules, SocketName);

}

void USSPWeaponComponent::EquipWeapon(ECharacterWeapon WeaponType)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
        if (WeaponType == ECharacterWeapon::ShotGun && CurrentWeapon == *MapWeapons.Find(WeaponType))
        {
            EquipAnimInProgress = true;
            PlayAnimMontage(UnequipAnimMontage);
        }
        else if (WeaponType == ECharacterWeapon::Riffle && CurrentWeapon == *MapWeapons.Find(WeaponType))
        {
            EquipAnimInProgress = true;
            PlayAnimMontage(EquipAnimMontage);
        }
    }

    CurrentWeapon = *MapWeapons.Find(WeaponType);
    const auto CurrentWeaponData = WeaponsData.Find(WeaponType);
    CurrentReloadAnimMontage = (CurrentWeaponData && CurrentWeaponData->HasReloadAnimMontage) ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
}

void USSPWeaponComponent::StartFire()
{
    if (!CanDoAction()) return;

    CurrentWeapon->StartFire();
}

void USSPWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;

    CurrentWeapon->StopFire();
}

void USSPWeaponComponent::NextWeapon()
{
    if(CanDoAction())
    {
        CurrentWeaponType = CurrentWeaponType == ECharacterWeapon::Riffle ? ECharacterWeapon::ShotGun : ECharacterWeapon ::Riffle;
        EquipWeapon(CurrentWeaponType);
    }
}

void USSPWeaponComponent::Reload()
{
    ChangeClip();
}

void USSPWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(AnimMontage);
}

void USSPWeaponComponent::InitAnimations()
{   
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USSPEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USSPWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("Equip anim notify is forgotten to set!"));
        checkNoEntry();
    }

    auto UnequipFinishedNotify = AnimUtils::FindNotifyByClass<USSPEquipFinishedAnimNotify>(UnequipAnimMontage);
    if (UnequipFinishedNotify)
    {
        UnequipFinishedNotify->OnNotified.AddUObject(this, &USSPWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("Equip anim notify is forgotten to set!"));
        checkNoEntry();
    }
    
    for (const auto& OneWeaponData : WeaponsData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USSPReloadFinishedAnimNotify>(OneWeaponData.Value.ReloadAnimMontage);
        if (!ReloadFinishedNotify)
        {
            continue;
            /*UE_LOG(WeaponComponentLog, Error, TEXT("Reload anim notify is forgotten to set!"));
            checkNoEntry();*/
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USSPWeaponComponent::OnReloadFinished);
    }
}

void USSPWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;
    
    EquipAnimInProgress = false;
    UE_LOG(WeaponComponentLog, Display, TEXT("OnEquipFinished!"));
}

void USSPWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;

    ReloadAnimInProgress = false;
    UE_LOG(WeaponComponentLog, Display, TEXT("OnReloadFinished!"));
}

bool USSPWeaponComponent::CanDoAction() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

void USSPWeaponComponent::OnEmptyClip()
{
    ChangeClip();    
}

void USSPWeaponComponent::ChangeClip()
{
    
    if (!CanDoAction() && !CurrentWeapon->CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();

    if (!CurrentReloadAnimMontage) return;
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}
