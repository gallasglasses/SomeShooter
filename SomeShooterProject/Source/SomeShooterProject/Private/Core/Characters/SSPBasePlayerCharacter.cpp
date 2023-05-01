


#include "Core/Characters/SSPBasePlayerCharacter.h"
#include "Core/Characters/Components/SSPCharacterMovementComponent.h"
#include "Core/Characters/Components/SSPHealthComponent.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "Core/Weapons/SSPBaseWeapon.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(BasePlayerCharacterLog, All, All);

// Sets default values
ASSPBasePlayerCharacter::ASSPBasePlayerCharacter(const FObjectInitializer& ObjInit):Super(
    ObjInit.SetDefaultSubobjectClass<USSPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = 300.0f;
    //SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    SpringArmComponent->SocketOffset = FVector(0.0f, 50.0f, 70.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USSPHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    BaseWeaponComponent = CreateDefaultSubobject<USSPWeaponComponent>("BaseWeaponComponent");
}

// Called when the game starts or when spawned
void ASSPBasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    USSPCharacterMovementComponent* PlayerCharacterMovement = Cast<USSPCharacterMovementComponent>(GetCharacterMovement());
    DefaultMaxSpeed = PlayerCharacterMovement->GetMaxSpeed();

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ASSPBasePlayerCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASSPBasePlayerCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASSPBasePlayerCharacter::OnGroundLanded);
}

// Called every frame
void ASSPBasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsMoving)
    {
        AccelerationTime += DeltaTime;
    }
    else
    {
        AccelerationTime = 0;
    }
}

// Called to bind functionality to input
void ASSPBasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(BaseWeaponComponent);

    //Axis Mapping
    PlayerInputComponent->BindAxis("MoveForward", this, &ASSPBasePlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASSPBasePlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASSPBasePlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASSPBasePlayerCharacter::AddControllerYawInput);

    //Action Mapping
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASSPBasePlayerCharacter::Jump);

    PlayerInputComponent->BindAction("Jog", IE_Pressed, this, &ASSPBasePlayerCharacter::SwitchWalkToJog);

    PlayerInputComponent->BindAction("Run", IE_DoubleClick, this, &ASSPBasePlayerCharacter::OnStartRunning);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASSPBasePlayerCharacter::OnStartSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASSPBasePlayerCharacter::OnStopSprinting);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASSPBasePlayerCharacter::CanFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, BaseWeaponComponent, &USSPWeaponComponent::StopFire);

    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, BaseWeaponComponent, &USSPWeaponComponent::NextWeapon);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, BaseWeaponComponent, &USSPWeaponComponent::Reload);
}

void ASSPBasePlayerCharacter::MoveForward(float Amount)
{
    bIsMoving = !GetVelocity().IsZero();

    USSPCharacterMovementComponent* PlayerCharacterMovement = Cast<USSPCharacterMovementComponent>(GetCharacterMovement());

    if (bIsMoving && (bIsJogging || bIsSprinting))
    {
        PlayerCharacterMovement->MaxWalkSpeed =
            FMath::FInterpTo(DefaultMaxSpeed, DefaultMaxSpeed * PlayerCharacterMovement->GetNewSpeed(), AccelerationTime, InterpSpeed);
    }
    else
    {
        PlayerCharacterMovement->MaxWalkSpeed = DefaultMaxSpeed;
    }

	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASSPBasePlayerCharacter::MoveRight(float Amount)
{
    bIsMoving = !GetVelocity().IsZero();

    USSPCharacterMovementComponent* PlayerCharacterMovement = Cast<USSPCharacterMovementComponent>(GetCharacterMovement());
    
    if (bIsMoving && (bIsJogging || bIsSprinting))
    {
        PlayerCharacterMovement->MaxWalkSpeed =
            FMath::FInterpTo(DefaultMaxSpeed, DefaultMaxSpeed * PlayerCharacterMovement->GetNewSpeed(), AccelerationTime, InterpSpeed);
    }
    else
    {
        PlayerCharacterMovement->MaxWalkSpeed = DefaultMaxSpeed;
    }

    AddMovementInput(GetActorRightVector(), Amount);
}

void ASSPBasePlayerCharacter::SwitchWalkToJog()
{
    bIsJogging = bIsWalking;
    bIsWalking = !bIsJogging;
}

void ASSPBasePlayerCharacter::OnStartRunning()
{
    bIsRunning = true;
}

void ASSPBasePlayerCharacter::OnStopRunning()
{
    bIsRunning = false;
}

void ASSPBasePlayerCharacter::OnStartSprinting()
{
    bIsSprinting = true;
    BaseWeaponComponent->StopFire();
}

void ASSPBasePlayerCharacter::OnStopSprinting()
{
    bIsSprinting = false;
}

void ASSPBasePlayerCharacter::CanFire()
{
    if (bIsSprinting) return;

    BaseWeaponComponent->StartFire();
}

bool ASSPBasePlayerCharacter::IsSprinting() const
{
    return bIsSprinting && !GetVelocity().IsZero();
}

bool ASSPBasePlayerCharacter::IsJogging() const
{
    return bIsJogging && !GetVelocity().IsZero();
}

bool ASSPBasePlayerCharacter::IsWalking() const
{
    return bIsWalking && !GetVelocity().IsZero();
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

void ASSPBasePlayerCharacter::OnDeath() 
{
    UE_LOG(BasePlayerCharacterLog, Display, TEXT("Player %s is dead!"), *GetName());

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BaseWeaponComponent->StopFire();
}

void ASSPBasePlayerCharacter::OnHealthChanged(float Health) 
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
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