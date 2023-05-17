#include "Core/Menu/SSPMenuPlayerController.h"

void ASSPMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
