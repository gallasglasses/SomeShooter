#include "Core/Weapons/Components/SSPWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponFXComponent, All, All);

USSPWeaponFXComponent::USSPWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USSPWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;
	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
			UE_LOG(LogWeaponFXComponent, Display, TEXT("Effect"));
		}
		else
		{
			UE_LOG(LogWeaponFXComponent, Display, TEXT("DefaultEffect"));
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		ImpactData.NiagaraEffect,
		Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation());

	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
		ImpactData.DecalData.Material,
		ImpactData.DecalData.Size,
		Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation());

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
	}
}

