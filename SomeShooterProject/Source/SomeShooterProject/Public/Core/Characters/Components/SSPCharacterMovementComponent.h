

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SSPCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOMESHOOTERPROJECT_API USSPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "3.0", ClampMax = "9.0"))
    float SprintModifier = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "3.0"))
    float JogModifier = 2.5f;

	//virtual float GetMaxSpeed() const override;
	float GetNewSpeed() const;
};
