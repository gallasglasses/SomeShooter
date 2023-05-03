


#include "Core/Pickups/SSPBasePickup.h"

// Sets default values
ASSPBasePickup::ASSPBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASSPBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASSPBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

