#include "Core/Weapons/SSPBaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ASSPBaseWeapon::ASSPBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASSPBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero!"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero!"));

    CurrentAmmo = DefaultAmmo;
}

void ASSPBaseWeapon::StartFire()
{
   
}

void ASSPBaseWeapon::StopFire()
{
    
}

void ASSPBaseWeapon::MakeShot() 
{
    
}

ACharacter* ASSPBaseWeapon::GetPlayer() const
{
    return !Cast<ACharacter>(GetOwner()) ? nullptr : Cast<ACharacter>(GetOwner());
}

APlayerController* ASSPBaseWeapon::GetPlayerController() const
{
    return !GetPlayer() ? nullptr : GetPlayer()->GetController<APlayerController>();
}

bool ASSPBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller)
        return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

void ASSPBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(BaseWeaponLog, Warning, TEXT("Clip is empty!"));
        return;
    }
    CurrentAmmo.Bullets--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast();
    }
}

bool ASSPBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASSPBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void ASSPBaseWeapon::ChangeClip()
{
    if(!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(BaseWeaponLog, Warning, TEXT("No more clips!"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    UE_LOG(BaseWeaponLog, Display, TEXT("--------------CHANGE CLIP--------------"));
    LogAmmo();
}

bool ASSPBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets || CurrentAmmo.Clips > 0;
}

void ASSPBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo);
}

//FVector ASSPBaseWeapon::GetMuzzleWorldLocation() const
//{
//
//}

bool ASSPBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;

    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASSPBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld())
        return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASSPBaseWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor)
        return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), GetPlayer());
}