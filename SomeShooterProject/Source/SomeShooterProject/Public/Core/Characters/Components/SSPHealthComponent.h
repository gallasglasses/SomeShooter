#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SSPHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOMESHOOTERPROJECT_API USSPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USSPHealthComponent();

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return Health / MaxHealth; }

	float GetHealth() const { return Health; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.00"))
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "AutoHeal"))
	float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;

	virtual void BeginPlay() override;

private:	
	float Health = 0.0f;

	FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, 
						 float Damage, 
						 const class UDamageType* DamageType, 
						 class AController* InstigatedBy, 
						 AActor* DamageCauser);

	void HealUpdate();
    void SetHealth(float NewHealth);
};
