#include "Core/Weapons/SSPRifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(RifleWeaponLog, All, All);

void ASSPRifleWeapon::StartFire()
{
    //UE_LOG(RifleWeaponLog, Display, TEXT("RifleFire!"));

    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASSPRifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASSPRifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

    UE_LOG(RifleWeaponLog, Display, TEXT("Stop RifleFire"));
}

void ASSPRifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    /*const auto Player = GetPlayer();
    if (!Player)
        return;*/

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        // UE_LOG(RifleWeaponLog, Display, TEXT("Hit Bone: %s"), *HitResult.BoneName.ToString());
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Silver, false, 3.0f, 0, 3.0f);
    }

    DecreaseAmmo();
}

bool ASSPRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;

    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

FVector ASSPRifleWeapon::GetMuzzleWorldLocation() const
{
    return GetPlayer()->GetMesh()->GetSocketLocation(MuzzleSocketName);
}