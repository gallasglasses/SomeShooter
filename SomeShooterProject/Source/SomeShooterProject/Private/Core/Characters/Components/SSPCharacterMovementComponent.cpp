


#include "Core/Characters/Components/SSPCharacterMovementComponent.h"
#include "Core/Characters/SSPBasePlayerCharacter.h"

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
    const ASSPBasePlayerCharacter* Player = Cast<ASSPBasePlayerCharacter>(GetPawnOwner());

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