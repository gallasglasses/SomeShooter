#include "Core/Weapons/SSPLauncherWeapon.h"
#include "Core/Weapons/SSPProjectile.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LauncherWeaponLog, All, All);

void ASSPLauncherWeapon::StartFire()
{
    //UE_LOG(LauncherWeaponLog, Display, TEXT("LauncherFire!"));

    MakeShot();
}

void ASSPLauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint + GetMuzzleWorldLocation().GetSafeNormal());
    
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASSPProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASSPProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetStartPosition(GetMuzzleWorldLocation());
        //Projectile->SetActorRotator(GetPlayer()->GetMesh()->GetSocketRotation(MuzzleSocketName));
        Projectile->SetActorRotator(GetPlayer()->GetBaseAimRotation());
        UE_LOG(LauncherWeaponLog, Display, TEXT("Direction: %s"), *Direction.ToString());

        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }

    DecreaseAmmo();
}

FVector ASSPLauncherWeapon::GetMuzzleWorldLocation() const
{
    return GetPlayer()->GetMesh()->GetSocketLocation(MuzzleSocketName);
}