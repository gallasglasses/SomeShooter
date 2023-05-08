#include "Core/AI/Services/SSPChangeWeaponService.h"
#include "Core/Characters/Components/SSPAIWeaponComponent.h"
#include "SSPUtils.h"

#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogChangeWeaponService, All, All);

USSPChangeWeaponService::USSPChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USSPChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();

	
	if (Controller)
	{
		const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			WeaponComponent->NextWeapon();
			UE_LOG(LogChangeWeaponService, Display, TEXT("Change Weapon!!!"));
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
