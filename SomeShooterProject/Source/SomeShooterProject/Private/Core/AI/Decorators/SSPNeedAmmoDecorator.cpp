#include "Core/AI/Decorators/SSPNeedAmmoDecorator.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "SSPUtils.h"

#include "AIController.h"

USSPNeedAmmoDecorator::USSPNeedAmmoDecorator()
{
	NodeName = "Need Ammo";
}

bool USSPNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent) return false;

	//UE_LOG(LogTemp, Display, TEXT("Need Ammo %s"), WeaponComponent->NeedAmmo(WeaponType) ? TEXT("true") : TEXT("false"));

	return WeaponComponent->NeedAmmo(WeaponType);
}
