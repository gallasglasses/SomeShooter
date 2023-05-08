#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSPBasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SOMESHOOTERPROJECT_API ASSPBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASSPBasePickup();

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
	UStaticMeshComponent* PickupMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime = 5.f;

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

public:	
	
	virtual void Tick(float DeltaTime) override;
	bool CouldBeTaken() const;

private:

	float RotationYaw = 0.0f;

	FTimerHandle RespawnTimerHandle;

	virtual bool GivePickupTo(APawn* PlayerPawn);

	void PickupWasTaken();
	void Respawn();
	void GenerateRotationYaw();

};
