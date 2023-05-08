#include "Core/AI/Services/SSPFindEnemyService.h"
#include "Core/Characters/Components/SSPAIPerceptionComponent.h"
#include "SSPUtils.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USSPFindEnemyService::USSPFindEnemyService()
{
	NodeName = "Find Enemy";
}

void USSPFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//UE_LOG(LogTemp, Display, TEXT("Find Enemy"));
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = SSPUtils::GetPlayerComponent<USSPAIPerceptionComponent>(Controller);
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}