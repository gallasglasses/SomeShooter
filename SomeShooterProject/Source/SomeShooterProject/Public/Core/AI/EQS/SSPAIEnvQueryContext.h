#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "SSPAIEnvQueryContext.generated.h"

UCLASS()
class SOMESHOOTERPROJECT_API USSPAIEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName EnemyActorKeyName = "EnemyActor";
};
