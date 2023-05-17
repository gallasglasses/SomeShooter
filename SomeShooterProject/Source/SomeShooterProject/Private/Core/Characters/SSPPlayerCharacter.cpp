#include "Core/Characters/SSPPlayerCharacter.h"
#include "Core/Characters/Components/SSPWeaponComponent.h"
#include "Core/Characters/Components/SSPCharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

ASSPPlayerCharacter::ASSPPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->TargetArmLength = 300.0f;
    //SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    SpringArmComponent->SocketOffset = FVector(0.0f, 50.0f, 70.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASSPPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASSPPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASSPPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ASSPPlayerCharacter::Tick(float DeltaTime)
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

void ASSPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(BaseWeaponComponent);

    //Axis Mapping
    PlayerInputComponent->BindAxis("MoveForward", this, &ASSPPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASSPPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASSPPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASSPPlayerCharacter::AddControllerYawInput);

    //Action Mapping
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASSPPlayerCharacter::Jump);

    PlayerInputComponent->BindAction("Jog", IE_Pressed, this, &ASSPPlayerCharacter::SwitchWalkToJog);

    PlayerInputComponent->BindAction("Run", IE_DoubleClick, this, &ASSPPlayerCharacter::OnStartRunning);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASSPPlayerCharacter::OnStartSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASSPPlayerCharacter::OnStopSprinting);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASSPPlayerCharacter::CanFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, BaseWeaponComponent, &USSPWeaponComponent::StopFire);

    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, BaseWeaponComponent, &USSPWeaponComponent::NextWeapon);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, BaseWeaponComponent, &USSPWeaponComponent::Reload);

    DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, BaseWeaponComponent, &USSPWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, BaseWeaponComponent, &USSPWeaponComponent::Zoom, false);
}

void ASSPPlayerCharacter::MoveForward(float Amount)
{
    bIsMoving = !GetVelocity().IsZero();

    USSPCharacterMovementComponent* PlayerCharacterMovement = Cast<USSPCharacterMovementComponent>(GetCharacterMovement());

    if (bIsMoving && (bIsJogging || bIsSprinting))
    {
        PlayerCharacterMovement->MaxWalkSpeed =
            FMath::FInterpTo(DefaultMaxSpeed, DefaultMaxSpeed * PlayerCharacterMovement->GetNewSpeed(), AccelerationTime, InterpSpeed);
    }
    else if (bIsMoving && bIsWalking)
    {
        PlayerCharacterMovement->MaxWalkSpeed = DefaultMaxSpeed;
    }

    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASSPPlayerCharacter::MoveRight(float Amount)
{
    bIsMoving = !GetVelocity().IsZero();

    USSPCharacterMovementComponent* PlayerCharacterMovement = Cast<USSPCharacterMovementComponent>(GetCharacterMovement());

    if (bIsMoving && (bIsJogging || bIsSprinting))
    {
        PlayerCharacterMovement->MaxWalkSpeed =
            FMath::FInterpTo(DefaultMaxSpeed, DefaultMaxSpeed * PlayerCharacterMovement->GetNewSpeed(), AccelerationTime, InterpSpeed);
    }
    else if (bIsMoving && bIsWalking)
    {
        PlayerCharacterMovement->MaxWalkSpeed = DefaultMaxSpeed;
    }

    AddMovementInput(GetActorRightVector(), Amount);
}

void ASSPPlayerCharacter::SwitchWalkToJog()
{
    bIsJogging = bIsWalking;
    bIsWalking = !bIsJogging;
}

void ASSPPlayerCharacter::OnStartRunning()
{
    bIsRunning = true;
}

void ASSPPlayerCharacter::OnStopRunning()
{
    bIsRunning = false;
}

void ASSPPlayerCharacter::OnStartSprinting()
{
    bIsSprinting = true;
    BaseWeaponComponent->StopFire();
}

void ASSPPlayerCharacter::OnStopSprinting()
{
    bIsSprinting = false;
}

void ASSPPlayerCharacter::CanFire()
{
    if (bIsSprinting) return;

    BaseWeaponComponent->StartFire();
}

void ASSPPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void ASSPPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASSPPlayerCharacter::CheckCameraOverlap()
{
    const auto NeedToHideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(NeedToHideMesh);
}

bool ASSPPlayerCharacter::IsSprinting() const
{
    return bIsSprinting && !GetVelocity().IsZero();
}

bool ASSPPlayerCharacter::IsJogging() const
{
    return bIsJogging && !GetVelocity().IsZero();
}

bool ASSPPlayerCharacter::IsWalking() const
{
    return bIsWalking && !GetVelocity().IsZero();
}

void ASSPPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}
