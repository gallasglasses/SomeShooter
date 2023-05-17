#include "Core/AI/SSPAIPlayerCharacter.h"
#include "Core/AI/SSPAIController.h"
#include "Core/Characters/Components/SSPAIWeaponComponent.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "Core/UI/SSPHealthBarWidget.h"
#include "SSPUtils.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"

ASSPAIPlayerCharacter::ASSPAIPlayerCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<USSPAIWeaponComponent>("BaseWeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ASSPAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
	}
}

void ASSPAIPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthWidgetComponent);
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

void ASSPAIPlayerCharacter::OnHealthChanged(float Health, float DeltaHealth)
{
	Super::OnHealthChanged(Health, DeltaHealth);

	const auto HealthBarWidget = Cast<USSPHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget) return;

	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
	HealthBarWidget->SetScaleDamage(DeltaHealth);
	if (IsPlayerInDistance())
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateHealthVisibilityTimer, this, &ASSPAIPlayerCharacter::UpdateHealthWidgetComponentByTimer, 2.0f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateHealthVisibilityTimer);
	}
}

bool ASSPAIPlayerCharacter::IsPlayerInDistance()
{
	if(!GetWorld() ||
	   !GetWorld()->GetFirstPlayerController() ||
	   !GetWorld()->GetFirstPlayerController()->GetPawn()) 
	   return false;

	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());

	return Distance < HealthVisibilityDistance;
}

void ASSPAIPlayerCharacter::UpdateHealthRotationByEnemy()
{
	if (!GetWorld() ||
		!GetWorld()->GetFirstPlayerController() ||
		!GetWorld()->GetFirstPlayerController()->GetPawn())
		return;

	const auto PlayerController = GetWorld()->GetFirstPlayerController();

	const FVector& HealthWidgetComponentPosition = HealthWidgetComponent->GetComponentLocation();
	const FVector& PlayerCameraPosition = PlayerController->PlayerCameraManager->GetCameraLocation();

	const FVector& AIForwardVector = GetActorForwardVector();
	const FVector& AIToCameraVector = PlayerCameraPosition - HealthWidgetComponentPosition;

	const FRotator& AIRotation = FRotationMatrix::MakeFromX(AIForwardVector).Rotator();
	const FRotator& TargetRotation = AIToCameraVector.Rotation();

	const float& Angle = TargetRotation.Yaw - AIRotation.Yaw;

	HealthWidgetComponent->SetWorldRotation(HealthWidgetComponent->GetComponentRotation() + FRotator(0.0f, Angle, 0.0f));
}

void ASSPAIPlayerCharacter::UpdateHealthWidgetComponentByTimer()
{
	UpdateHealthRotationByEnemy();
	HealthWidgetComponent->SetVisibility(IsPlayerInDistance(), true);
}
