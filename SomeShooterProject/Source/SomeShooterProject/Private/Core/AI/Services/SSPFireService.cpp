#include "Core/AI/Services/SSPFireService.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "SSPUtils.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFireService, All, All);

USSPFireService::USSPFireService()
{
	NodeName = "Fire";
}

void USSPFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent(); 
	const auto Controller = OwnerComp.GetAIOwner();

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (Controller)
	{
		const auto WeaponComponent = SSPUtils::GetPlayerComponent<USSPWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
			UE_LOG(LogFireService, Display, TEXT("HasAim %s"), HasAim ? TEXT("true") : TEXT("false"));
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
