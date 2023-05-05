

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSPProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class USphereComponent;
class USSPWeaponFXComponent;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ASSPProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction;}
	void SetStartPosition(const FVector& Position) { StartPosition = Position; }
	void SetActorRotator(const FRotator& Rotator) { ActorRotator = Rotator; }

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float LifeSeconds = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		USSPWeaponFXComponent* WeaponFXComponent;

	virtual void BeginPlay() override;

private:

	FVector ShotDirection;
	FVector StartPosition;
	FRotator ActorRotator;

	UFUNCTION()
		void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
	FVector GetProjectileMovementDirection();
};
