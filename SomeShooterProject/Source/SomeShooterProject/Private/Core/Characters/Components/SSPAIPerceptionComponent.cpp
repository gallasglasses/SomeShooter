#include "Core/Characters/Components/SSPAIPerceptionComponent.h"
#include "AIController.h"
#include "SSPUtils.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USSPAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	if (PercieveActors.Num() == 0) return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	for (const auto PercieveActor : PercieveActors)
	{
		const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(PercieveActor);
		if (HealthComponent && !HealthComponent->IsDead())
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}
	
	return BestPawn;
}
