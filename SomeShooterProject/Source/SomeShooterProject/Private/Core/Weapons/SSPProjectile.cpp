#include "Core/Weapons/SSPProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(ProjectileLog, All, All);

ASSPProjectile::ASSPProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(CollisionComponent);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void ASSPProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);

	CollisionComponent->SetRelativeRotation((CollisionComponent->GetRelativeRotation()) + ActorRotator);

	//MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	MovementComponent->Velocity = GetActorForwardVector() * MovementComponent->InitialSpeed;
	UE_LOG(ProjectileLog, Display, TEXT("Velocity: %s"), *MovementComponent->Velocity.ToString());

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	UE_LOG(ProjectileLog, Display, TEXT("Owner: %s"), GetOwner());

	CollisionComponent->OnComponentHit.AddDynamic(this, &ASSPProjectile::OnProjectileHit);

	SetLifeSpan(LifeSeconds);
}

FVector ASSPProjectile::GetProjectileMovementDirection()
{
	const auto ShotDirectionNormal = ShotDirection.GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), ShotDirectionNormal));
	
	float Degrees = FMath::RadiansToDegrees(AngleBetween);
	FVector ProjectileDirection;
	return ProjectileDirection;
}

void ASSPProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	UGameplayStatics::ApplyRadialDamage(GetWorld(), //
		DamageAmount, //
		GetActorLocation(), //
		DamageRadius, //
		UDamageType::StaticClass(), //
		{GetOwner()}, //
		this, //
		GetController(), //
		DoFullDamage);

	DrawDebugSphere(GetWorld(),GetActorLocation(), DamageRadius, 24, FColor::Magenta, false, 5.0f);

	Destroy();
}

AController* ASSPProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
