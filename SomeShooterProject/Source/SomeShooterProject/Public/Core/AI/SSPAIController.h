#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SSPAIController.generated.h"

class USSPAIPerceptionComponent;
class USSPRespawnComponent;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPAIController : public AAIController
{
	GENERATED_BODY()

public:

	ASSPAIController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USSPAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USSPRespawnComponent* RespawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName FocusOnKeyName = "EnemyActor";

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	
private:

	AActor* GetFocusOnActor() const;
};
