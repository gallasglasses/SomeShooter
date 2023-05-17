#pragma once

#include "CoreMinimal.h"
#include "Core/Characters/SSPBasePlayerCharacter.h"
#include "SSPPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPPlayerCharacter : public ASSPBasePlayerCharacter
{
	GENERATED_BODY()

public:

	ASSPPlayerCharacter(const FObjectInitializer& ObjInit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USphereComponent* CameraCollisionComponent;

	virtual void OnDeath() override;
	virtual void BeginPlay() override;


public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsSprinting() const override;
	virtual bool IsJogging() const override;
	virtual bool IsWalking() const override;

private:

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void SwitchWalkToJog();
	void OnStartRunning();
	void OnStopRunning();
	void OnStartSprinting();
	void OnStopSprinting();
	void CanFire();

	bool bIsWalking = false;
	bool bIsJogging = true;
	bool bIsRunning = false;
	bool bIsSprinting = false;
	bool bIsMoving = false;

	float AccelerationTime = 0.0f;
	
	UFUNCTION()
		void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
										   AActor* OtherActor,
										   UPrimitiveComponent* OtherComp,
										   int32 OtherBodyIndex,
										   bool bFromSweep,
										   const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
										 AActor* OtherActor,
										 UPrimitiveComponent* OtherComp,
										 int32 OtherBodyIndex);

	void CheckCameraOverlap();
};
