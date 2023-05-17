#include "Core/Characters/SSPBasePlayerController.h"
#include "Core/Characters/Components/SSPRespawnComponent.h"
#include "SomeShooterProjectGameModeBase.h"

ASSPBasePlayerController::ASSPBasePlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USSPRespawnComponent>("RespawnComponent");
}

void ASSPBasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		const auto GameMode = Cast<ASomeShooterProjectGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASSPBasePlayerController::OnMatchStateChanged);
		}
	}
}

void ASSPBasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASSPBasePlayerController::OnPauseGame);
}

void ASSPBasePlayerController::OnPauseGame()
{
	if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASSPBasePlayerController::OnMatchStateChanged(ESSPMatchState State)
{
	if (State == ESSPMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}
