#include "Core/AI/SSPAIPlayerCharacter.h"
#include "Core/AI/SSPAIController.h"
#include "Core/Characters/Components/SSPAIWeaponComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"

ASSPAIPlayerCharacter::ASSPAIPlayerCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<USSPAIWeaponComponent>("BaseWeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASSPAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
	}
}

void ASSPAIPlayerCharacter::OnDeath()
{
	Super::OnDeath();

	const auto AIController = Cast<AAIController>(Controller);
	if (AIController && AIController->BrainComponent)
	{
		AIController->BrainComponent->Cleanup();
	}
}
