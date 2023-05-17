#include "Core/Characters/Components/SSPCharacterMovementComponent.h"
#include "Core/Characters/SSPBasePlayerCharacter.h"
#include "Core/AI/SSPAIPlayerCharacter.h"

#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogMovementComponent, All, All);

/* float USSPCharacterMovementComponent::GetMaxSpeed() const
{
	const ASSPBasePlayerCharacter* Player = Cast<ASSPBasePlayerCharacter>(GetPawnOwner());

    const float MaxSpeed = Super::GetMaxSpeed();

    if (Player && Player->IsJogging() && !Player->IsWalking() && !Player->IsSprinting())
    {
        return MaxSpeed * JogModifier;
    }
    else if (Player && Player->IsSprinting() &&
             ((Player->IsWalking() && !Player->IsJogging()) || (Player->IsJogging() && !Player->IsWalking())))
    {
        return MaxSpeed * SprintModifier;
    }
    else
    {
        return MaxSpeed;
    }
}*/

float USSPCharacterMovementComponent::GetNewSpeed() const
{
    //UE_LOG(LogMovementComponent, Display, TEXT("Player is AController!"));

    const auto Player = Cast<ASSPBasePlayerCharacter>(GetOwner());

    const float MaxSpeed = Super::GetMaxSpeed();

    if (Player && Player->IsJogging() && !Player->IsWalking() && !Player->IsSprinting())
    {
        return JogModifier;
    }
    else if (Player && Player->IsSprinting() &&
        ((Player->IsWalking() && !Player->IsJogging()) || (Player->IsJogging() && !Player->IsWalking())))
    {
        return SprintModifier;
    }
    else
    {
        return 1.0f;
    }
}