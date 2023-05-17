


#include "Core/Characters/SSPBasePlayerCharacter.h"
#include "Core/Characters/Components/SSPCharacterMovementComponent.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "Core/Weapons/SSPBaseWeapon.h"
#include "Core/UI/SSPHealthBarWidget.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BasePlayerCharacterLog, All, All);

ASSPBasePlayerCharacter::ASSPBasePlayerCharacter(const FObjectInitializer& ObjInit):Super(
    ObjInit.SetDefaultSubobjectClass<USSPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USSPHealthComponent>("HealthComponent");

    BaseWeaponComponent = CreateDefaultSubobject<USSPWeaponComponent>("BaseWeaponComponent");

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
    HealthWidgetComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
    HealthWidgetComponent->SetOwnerNoSee(true);
}

void ASSPBasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    check(GetCharacterMovement());
    check(BaseWeaponComponent);
    check(GetMesh());

    USSPCharacterMovementComponent* PlayerCharacterMovement = Cast<USSPCharacterMovementComponent>(GetCharacterMovement());
    DefaultMaxSpeed = PlayerCharacterMovement->GetMaxSpeed();

    OnHealthChanged(HealthComponent->GetHealth(), 0.f);
    HealthComponent->OnDeath.AddUObject(this, &ASSPBasePlayerCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASSPBasePlayerCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASSPBasePlayerCharacter::OnGroundLanded);
}

void ASSPBasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASSPBasePlayerCharacter::IsSprinting() const
{
    return false;
}

bool ASSPBasePlayerCharacter::IsJogging() const
{
    return false;
}

bool ASSPBasePlayerCharacter::IsWalking() const
{
    return false;
}

float ASSPBasePlayerCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
    {
        return 0;
    }
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASSPBasePlayerCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto NumMaterials = GetMesh()->GetMaterials().Num();
    
    for (size_t i = 0; i < NumMaterials; i++)
    {
        const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
        if(!MaterialInst) return;

        MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
    }

    //// for one material in mesh
    //const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    //if(!MaterialInst) return;

    //MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}

void ASSPBasePlayerCharacter::SetHealthBarStyle(const FProgressBarStyle& BarStyle)
{
    const auto HealthBarWidget = Cast<USSPHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget) return;

    HealthBarWidget->SetHealthBarStyle(BarStyle);
}

void ASSPBasePlayerCharacter::OnDeath() 
{
    UE_LOG(BasePlayerCharacterLog, Display, TEXT("Player %s is dead!"), *GetName());

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BaseWeaponComponent->StopFire();
    BaseWeaponComponent->Zoom(false);

    // Ragdoll: check PhysicsAsset
    //It was for fun and it looked very ridiculous
    /*GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);*/
}

void ASSPBasePlayerCharacter::OnHealthChanged(float Health, float DeltaHealth)
{
    
}

void ASSPBasePlayerCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(BasePlayerCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X) return;
    
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(BasePlayerCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);

    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}