#include "Core/Weapons/SSPRifleWeapon.h"
#include "Core/Weapons/Components/SSPWeaponFXComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(RifleWeaponLog, All, All);

ASSPRifleWeapon::ASSPRifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USSPWeaponFXComponent>("WeaponFXComponent");
}

void ASSPRifleWeapon::StartFire()
{
    UE_LOG(RifleWeaponLog, Display, TEXT("RifleFire!"));

    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASSPRifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASSPRifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);

    UE_LOG(RifleWeaponLog, Display, TEXT("Stop RifleFire"));
}

void ASSPRifleWeapon::Zoom(bool Enabled)
{
    const auto Controller = Cast<APlayerController>(GetPlayerController());
    if(!Controller || !Controller->PlayerCameraManager) return;

    if (Enabled)
    {
        DefaultCameraFOV = Controller->PlayerCameraManager ->GetFOVAngle();
    }

    Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}

void ASSPRifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASSPRifleWeapon::MakeShot()
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

    SpawnMuzzleParticle();

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd;

    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);

        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        // UE_LOG(RifleWeaponLog, Display, TEXT("Hit Bone: %s"), *HitResult.BoneName.ToString());

        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    /*else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Silver, false, 3.0f, 0, 3.0f);
    }*/

    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
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

void ASSPRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}
