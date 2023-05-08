#include "Core/Weapons/SSPBaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

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
    const auto SSPCharacter = Cast<ACharacter>(GetOwner());
    if (!SSPCharacter) return false;

    if (SSPCharacter->IsPlayerControlled())
    {
        const auto Controller = GetPlayerController();
        if (!Controller) return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleWorldLocation();
        //ViewRotation = GetMuzzleWorldTransform().GetRotation().Rotator();
        ViewRotation = GetPlayer()->GetMesh()->GetSocketRotation(MuzzleSocketName);
    }
    
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
    //LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
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
    //LogAmmo();
}

bool ASSPBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets || CurrentAmmo.Clips > 0;
}

bool ASSPBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if(CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <=0) return false;
    if (IsAmmoEmpty())
    {
        UE_LOG(BaseWeaponLog, Display, TEXT("Ammo Is Empty"));
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
            UE_LOG(BaseWeaponLog, Display, TEXT("Clips were added"));
        }
        else
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
            UE_LOG(BaseWeaponLog, Display, TEXT("Ammo Is Full now"));
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        UE_LOG(BaseWeaponLog, Display, TEXT("Bullets were added"));
    }
    return true;
}

FVector ASSPBaseWeapon::GetMuzzleWorldLocation() const
{
    return GetPlayer()->GetMesh()->GetSocketLocation(MuzzleSocketName);
}

void ASSPBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo);
}

FTransform ASSPBaseWeapon::GetMuzzleWorldTransform() const
{
    return GetPlayer()->GetMesh()->GetSocketTransform(MuzzleSocketName);
}

void ASSPBaseWeapon::SpawnMuzzleParticle()
{
    FTransform SpawnTransform = GetMuzzleWorldTransform();
    SpawnTransform.SetScale3D(SpawnTransform.GetScale3D() * ParticleScale);
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticleSystem, SpawnTransform, true);
}

bool ASSPBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Bullets == DefaultAmmo.Bullets && CurrentAmmo.Clips == DefaultAmmo.Clips;
}

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
    CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASSPBaseWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor)
        return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), GetPlayer());
}