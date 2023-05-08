#include "Core/Pickups/SSPBasePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

ASSPBasePickup::ASSPBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetupAttachment(CollisionComponent);
}

void ASSPBasePickup::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionComponent);
	check(PickupMesh);

	GenerateRotationYaw();
}

void ASSPBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotationYaw, 0.f));
}

bool ASSPBasePickup::CouldBeTaken() const
{
	return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}

bool ASSPBasePickup::GivePickupTo(APawn* PlayerPawn)
{
	return false;
}

void ASSPBasePickup::PickupWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASSPBasePickup::Respawn, RespawnTime);
}

void ASSPBasePickup::Respawn()
{
	GenerateRotationYaw();
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
}

void ASSPBasePickup::GenerateRotationYaw()
{
	const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
	RotationYaw = FMath::RandRange(1.0f,2.0f) * Direction;
}

void ASSPBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Pawn = Cast<APawn>(OtherActor);
	if (GivePickupTo(Pawn))
	{
		PickupWasTaken();
		UE_LOG(LogBasePickup, Display, TEXT("Was taken!"));
	}
	else
	{
		UE_LOG(LogBasePickup, Display, TEXT("Wasn't taken!"));
	}
}

