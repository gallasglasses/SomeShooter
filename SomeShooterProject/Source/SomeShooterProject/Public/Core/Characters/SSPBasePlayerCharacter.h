#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSPBasePlayerCharacter.generated.h"

class UAnimMontage;
class UWidgetComponent;

class USSPHealthComponent;
class USSPWeaponComponent;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPBasePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASSPBasePlayerCharacter(const FObjectInitializer& ObjInit);

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USSPHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USSPWeaponComponent* BaseWeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animations")
        UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
        float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
        FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
        FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Material")
        FName MaterialColorName = "BodyMetalColor";

    UPROPERTY(EditAnywhere, Category = "Movement")
        float InterpSpeed = 10.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UWidgetComponent* HealthWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
        float HealthVisibilityDistance = 1000.0f;

	virtual void BeginPlay() override;

    virtual void OnDeath();

    virtual void OnHealthChanged(float Health, float DeltaHealth);

    float DefaultMaxSpeed = 0.0f;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsJogging() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsWalking() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    void SetPlayerColor(const FLinearColor& Color);
    void SetHealthBarStyle(const FProgressBarStyle& BarStyle);

private:

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
