

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SSPBasePlayerCharacter.generated.h"

class UAnimMontage;
class UCameraComponent;
class UCharacterMovementComponent;
class USpringArmComponent;
class UTextRenderComponent;

class USSPHealthComponent;
class USSPWeaponComponent;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPBasePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASSPBasePlayerCharacter(const FObjectInitializer& ObjInit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USSPHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UTextRenderComponent* HealthTextComponent;

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void OnDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsJogging() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsWalking() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:

    UPROPERTY(EditAnywhere, Category = "Movement")
    float InterpSpeed = 10.0f;

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void SwitchWalkToJog();
    void OnStartRunning();
    void OnStopRunning();
    void OnStartSprinting();
    void OnStopSprinting();
    void CanFire();
    void OnHealthChanged(float Health, float DeltaHealth);

	bool bIsWalking = false;
    bool bIsJogging = true;
    bool bIsRunning = false;
    bool bIsSprinting = false;
    bool bIsMoving = false;

    float DefaultMaxSpeed = 0.0f;
    float AccelerationTime = 0.0f;
};
