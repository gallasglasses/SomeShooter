#include "Core/AI/Decorators/SSPHealthPercentDecorator.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "SSPUtils.h"

#include "AIController.h"

USSPHealthPercentDecorator::USSPHealthPercentDecorator()
{
	NodeName = "Health Percent";
}


bool USSPHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const auto HealthComponent = SSPUtils::GetPlayerComponent<USSPHealthComponent>(Controller->GetPawn());
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	//UE_LOG(LogTemp, Display, TEXT("Find Health %s"), (HealthComponent->GetHealthPercent() <= HealthPercent) ? TEXT("true") : TEXT("false"));
	
	return HealthComponent->GetHealthPercent() <= HealthPercent;
}