#include "Core/AI/SSPAIController.h"
#include "Core/AI/SSPAIPlayerCharacter.h"
#include "Core/Characters/Components/SSPAIPerceptionComponent.h"
#include "Core/Characters/Components/SSPRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASSPAIController::ASSPAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<USSPAIPerceptionComponent>("AIPerceptionComponent");
	SetPerceptionComponent(*AIPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<USSPRespawnComponent>("RespawnComponent");

	bWantsPlayerState = true;
}

void ASSPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto AICharacter = Cast<ASSPAIPlayerCharacter>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTree);
	}
}

void ASSPAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ASSPAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
