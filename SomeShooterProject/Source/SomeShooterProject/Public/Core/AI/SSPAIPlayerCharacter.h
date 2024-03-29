#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/SSPBasePlayerCharacter.h"
#include "SSPAIPlayerCharacter.generated.h"

class UBehaviorTree;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPAIPlayerCharacter : public ASSPBasePlayerCharacter
{
	GENERATED_BODY()

public:

	ASSPAIPlayerCharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		UBehaviorTree* BehaviorTree;

protected:

	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual void OnHealthChanged(float Health, float DeltaHealth) override;

private:

	FTimerHandle UpdateHealthVisibilityTimer;

	bool IsPlayerInDistance();
	void UpdateHealthRotationByEnemy();
	void UpdateHealthWidgetComponentByTimer();
};
