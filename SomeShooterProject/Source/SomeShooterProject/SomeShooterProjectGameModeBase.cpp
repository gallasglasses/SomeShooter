// Copyright Epic Games, Inc. All Rights Reserved.


#include "SomeShooterProjectGameModeBase.h"
#include "Core/Characters/SSPBasePlayerCharacter.h"
#include "Core/Characters/SSPBasePlayerController.h"

ASomeShooterProjectGameModeBase::ASomeShooterProjectGameModeBase()
{	
	DefaultPawnClass = ASSPBasePlayerCharacter::StaticClass();
    PlayerControllerClass = ASSPBasePlayerController::StaticClass();
}

