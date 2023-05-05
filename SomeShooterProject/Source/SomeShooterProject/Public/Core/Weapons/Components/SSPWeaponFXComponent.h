#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSPCoreTypes.h"
#include "SSPWeaponFXComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOMESHOOTERPROJECT_API USSPWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	

	USSPWeaponFXComponent();
	
	void PlayImpactFX(const FHitResult& Hit);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FImpactData DefaultImpactData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};
