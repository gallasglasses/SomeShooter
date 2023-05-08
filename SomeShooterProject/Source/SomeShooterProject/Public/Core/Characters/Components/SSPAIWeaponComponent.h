#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "SSPAIWeaponComponent.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPAIWeaponComponent : public USSPWeaponComponent
{
	GENERATED_BODY()
	
public:

	virtual void StartFire() override;
	virtual void NextWeapon() override;
};
